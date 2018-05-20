//===-- ODRHash.cpp - Hashing to diagnose ODR failures ----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file implements the ODRHash class, which calculates a hash based
/// on AST nodes, which is stable across different runs.
///
//===----------------------------------------------------------------------===//

#include "clang/AST/ODRHash.h"

#include "clang/AST/DeclVisitor.h"
#include "clang/AST/NestedNameSpecifier.h"
#include "clang/AST/StmtVisitor.h"
#include "clang/AST/TypeVisitor.h"

using namespace clang;

void ODRHash::AddStmt(const Stmt *S) {
  assert(S && "Expecting non-null pointer.");
  S->ProcessODRHash(ID, *this);
}

void ODRHash::AddIdentifierInfo(const IdentifierInfo *II) {
  assert(II && "Expecting non-null pointer.");
  ID.AddString(II->getName());
}

void ODRHash::AddDeclarationName(DeclarationName Name) {
  AddBoolean(Name.isEmpty());
  if (Name.isEmpty())
    return;

  auto Kind = Name.getNameKind();
  ID.AddInteger(Kind);
  switch (Kind) {
  case DeclarationName::Identifier:
    AddIdentifierInfo(Name.getAsIdentifierInfo());
    break;
  case DeclarationName::ObjCZeroArgSelector:
  case DeclarationName::ObjCOneArgSelector:
  case DeclarationName::ObjCMultiArgSelector: {
    Selector S = Name.getObjCSelector();
    AddBoolean(S.isNull());
    AddBoolean(S.isKeywordSelector());
    AddBoolean(S.isUnarySelector());
    unsigned NumArgs = S.getNumArgs();
    for (unsigned i = 0; i < NumArgs; ++i) {
      AddIdentifierInfo(S.getIdentifierInfoForSlot(i));
    }
    break;
  }
  case DeclarationName::CXXConstructorName:
  case DeclarationName::CXXDestructorName:
    AddQualType(Name.getCXXNameType());
    break;
  case DeclarationName::CXXOperatorName:
    ID.AddInteger(Name.getCXXOverloadedOperator());
    break;
  case DeclarationName::CXXLiteralOperatorName:
    AddIdentifierInfo(Name.getCXXLiteralIdentifier());
    break;
  case DeclarationName::CXXConversionFunctionName:
    AddQualType(Name.getCXXNameType());
    break;
  case DeclarationName::CXXUsingDirective:
    break;
  case DeclarationName::CXXDeductionGuideName: {
    auto *Template = Name.getCXXDeductionGuideTemplate();
    AddBoolean(Template);
    if (Template) {
      AddDecl(Template);
    }
  }
  }
}

void ODRHash::AddNestedNameSpecifier(const NestedNameSpecifier *NNS) {}
void ODRHash::AddTemplateName(TemplateName Name) {}
void ODRHash::AddTemplateArgument(TemplateArgument TA) {}
void ODRHash::AddTemplateParameterList(const TemplateParameterList *TPL) {}

void ODRHash::clear() {
  DeclMap.clear();
  TypeMap.clear();
  Bools.clear();
  ID.clear();
}

unsigned ODRHash::CalculateHash() {
  // Append the bools to the end of the data segment backwards.  This allows
  // for the bools data to be compressed 32 times smaller compared to using
  // ID.AddBoolean
  const unsigned unsigned_bits = sizeof(unsigned) * CHAR_BIT;
  const unsigned size = Bools.size();
  const unsigned remainder = size % unsigned_bits;
  const unsigned loops = size / unsigned_bits;
  auto I = Bools.rbegin();
  unsigned value = 0;
  for (unsigned i = 0; i < remainder; ++i) {
    value <<= 1;
    value |= *I;
    ++I;
  }
  ID.AddInteger(value);

  for (unsigned i = 0; i < loops; ++i) {
    value = 0;
    for (unsigned j = 0; j < unsigned_bits; ++j) {
      value <<= 1;
      value |= *I;
      ++I;
    }
    ID.AddInteger(value);
  }

  assert(I == Bools.rend());
  Bools.clear();
  return ID.ComputeHash();
}

// Process a Decl pointer.  Add* methods call back into ODRHash while Visit*
// methods process the relevant parts of the Decl.
class ODRDeclVisitor : public ConstDeclVisitor<ODRDeclVisitor> {
  typedef ConstDeclVisitor<ODRDeclVisitor> Inherited;
  llvm::FoldingSetNodeID &ID;
  ODRHash &Hash;

public:
  ODRDeclVisitor(llvm::FoldingSetNodeID &ID, ODRHash &Hash)
      : ID(ID), Hash(Hash) {}

  void AddStmt(const Stmt *S) {
    Hash.AddBoolean(S);
    if (S) {
      Hash.AddStmt(S);
    }
  }

  void AddIdentifierInfo(const IdentifierInfo *II) {
    Hash.AddBoolean(II);
    if (II) {
      Hash.AddIdentifierInfo(II);
    }
  }

  void AddQualType(QualType T) {
    Hash.AddQualType(T);
  }

  void Visit(const Decl *D) {
    ID.AddInteger(D->getKind());
    Inherited::Visit(D);
  }

  void VisitNamedDecl(const NamedDecl *D) {
    Hash.AddDeclarationName(D->getDeclName());
    Inherited::VisitNamedDecl(D);
  }

  void VisitValueDecl(const ValueDecl *D) {
    AddQualType(D->getType());
    Inherited::VisitValueDecl(D);
  }

  void VisitAccessSpecDecl(const AccessSpecDecl *D) {
    ID.AddInteger(D->getAccess());
    Inherited::VisitAccessSpecDecl(D);
  }

  void VisitStaticAssertDecl(const StaticAssertDecl *D) {
    AddStmt(D->getAssertExpr());
    AddStmt(D->getMessage());

    Inherited::VisitStaticAssertDecl(D);
  }

  void VisitFieldDecl(const FieldDecl *D) {
    const bool IsBitfield = D->isBitField();
    Hash.AddBoolean(IsBitfield);

    if (IsBitfield) {
      AddStmt(D->getBitWidth());
    }

    Hash.AddBoolean(D->isMutable());
    AddStmt(D->getInClassInitializer());

    Inherited::VisitFieldDecl(D);
  }

  void VisitFunctionDecl(const FunctionDecl *D) {
    ID.AddInteger(D->getStorageClass());
    Hash.AddBoolean(D->isInlineSpecified());
    Hash.AddBoolean(D->isVirtualAsWritten());
    Hash.AddBoolean(D->isPure());
    Hash.AddBoolean(D->isDeletedAsWritten());

    Inherited::VisitFunctionDecl(D);
  }

  void VisitCXXMethodDecl(const CXXMethodDecl *D) {
    Hash.AddBoolean(D->isConst());
    Hash.AddBoolean(D->isVolatile());

    Inherited::VisitCXXMethodDecl(D);
  }
};

// Only allow a small portion of Decl's to be processed.  Remove this once
// all Decl's can be handled.
bool ODRHash::isWhitelistedDecl(const Decl *D, const CXXRecordDecl *Parent) {
  if (D->isImplicit()) return false;
  if (D->getDeclContext() != Parent) return false;

  switch (D->getKind()) {
    default:
      return false;
    case Decl::AccessSpec:
    case Decl::CXXMethod:
    case Decl::Field:
    case Decl::StaticAssert:
      return true;
  }
}

void ODRHash::AddSubDecl(const Decl *D) {
  assert(D && "Expecting non-null pointer.");
  AddDecl(D);

  ODRDeclVisitor(ID, *this).Visit(D);
}

void ODRHash::AddCXXRecordDecl(const CXXRecordDecl *Record) {
  assert(Record && Record->hasDefinition() &&
         "Expected non-null record to be a definition.");
  AddDecl(Record);

  // Filter out sub-Decls which will not be processed in order to get an
  // accurate count of Decl's.
  llvm::SmallVector<const Decl *, 16> Decls;
  for (const Decl *SubDecl : Record->decls()) {
    if (isWhitelistedDecl(SubDecl, Record)) {
      Decls.push_back(SubDecl);
    }
  }

  ID.AddInteger(Decls.size());
  for (auto SubDecl : Decls) {
    AddSubDecl(SubDecl);
  }
}

void ODRHash::AddDecl(const Decl *D) {
  assert(D && "Expecting non-null pointer.");
  auto Result = DeclMap.insert(std::make_pair(D, DeclMap.size()));
  ID.AddInteger(Result.first->second);
  // On first encounter of a Decl pointer, process it.  Every time afterwards,
  // only the index value is needed.
  if (!Result.second) {
    return;
  }

  ID.AddInteger(D->getKind());

  if (const NamedDecl *ND = dyn_cast<NamedDecl>(D)) {
    AddDeclarationName(ND->getDeclName());
  }
}

// Process a Type pointer.  Add* methods call back into ODRHash while Visit*
// methods process the relevant parts of the Type.
class ODRTypeVisitor : public TypeVisitor<ODRTypeVisitor> {
  typedef TypeVisitor<ODRTypeVisitor> Inherited;
  llvm::FoldingSetNodeID &ID;
  ODRHash &Hash;

public:
  ODRTypeVisitor(llvm::FoldingSetNodeID &ID, ODRHash &Hash)
      : ID(ID), Hash(Hash) {}

  void AddStmt(Stmt *S) {
    Hash.AddBoolean(S);
    if (S) {
      Hash.AddStmt(S);
    }
  }

  void AddDecl(Decl *D) {
    Hash.AddBoolean(D);
    if (D) {
      Hash.AddDecl(D);
    }
  }

  void Visit(const Type *T) {
    ID.AddInteger(T->getTypeClass());
    Inherited::Visit(T);
  }

  void VisitType(const Type *T) {}

  void VisitBuiltinType(const BuiltinType *T) {
    ID.AddInteger(T->getKind());
    VisitType(T);
  }

  void VisitTypedefType(const TypedefType *T) {
    AddDecl(T->getDecl());
    VisitType(T);
  }
};

void ODRHash::AddType(const Type *T) {
  assert(T && "Expecting non-null pointer.");
  auto Result = TypeMap.insert(std::make_pair(T, TypeMap.size()));
  ID.AddInteger(Result.first->second);
  // On first encounter of a Type pointer, process it.  Every time afterwards,
  // only the index value is needed.
  if (!Result.second) {
    return;
  }

  ODRTypeVisitor(ID, *this).Visit(T);
}

void ODRHash::AddQualType(QualType T) {
  AddBoolean(T.isNull());
  if (T.isNull())
    return;
  SplitQualType split = T.split();
  ID.AddInteger(split.Quals.getAsOpaqueValue());
  AddType(split.Ty);
}

void ODRHash::AddBoolean(bool Value) {
  Bools.push_back(Value);
}
