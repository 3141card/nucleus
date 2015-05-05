/**
 * (c) 2015 Nucleus project. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#pragma once

#include "nucleus/common.h"

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/LLVMContext.h"

namespace cpu {
namespace hir {

typedef size_t TypeSize;

class Type
{
public:
    llvm::Type* type;
};

// Integer types
struct I1 {
    using type = bool;
    static const TypeSize size = 1;
    static Type getType() {
        return Type{ llvm::Type::getInt1Ty(llvm::getGlobalContext()) };
    }
};

struct I8 {
    using type = u8;
    static const TypeSize size = 8;
    static Type getType() {
        return Type{ llvm::Type::getInt8Ty(llvm::getGlobalContext()) };
    }
};

struct I16 {
    using type = u16;
    static const TypeSize size = 16;
    static Type getType() {
        return Type{ llvm::Type::getInt16Ty(llvm::getGlobalContext()) };
    }
};

struct I32 {
    using type = u32;
    static const TypeSize size = 32;
    static Type getType() {
        return Type{ llvm::Type::getInt32Ty(llvm::getGlobalContext()) };
    }
};

struct I64 {
    using type = u64;
    static const TypeSize size = 64;
    static Type getType() {
        return Type{ llvm::Type::getInt64Ty(llvm::getGlobalContext()) };
    }
};

struct I128 {
    using type = u128;
    static const TypeSize size = 128;
    static Type getType() {
        return Type{ llvm::Type::getIntNTy(llvm::getGlobalContext(), 128) };
    }
};

// Floating-point types
struct F32 {
    using cmp = I1;
    using type = f32;
    static const TypeSize size = 32;
    static Type getType() {
        return Type{ llvm::Type::getFloatTy(llvm::getGlobalContext()) };
    }
};

struct F64 {
    using type = f64;
    static const TypeSize size = 64;
    static Type getType() {
        return Type{ llvm::Type::getDoubleTy(llvm::getGlobalContext()) };
    }
};

// Aggregate types
template <typename T, int N>
struct Array {
    using type = typename T::type[N];
    static const TypeSize size = N * T::size;
    static Type getType() {
        return Type{ llvm::ArrayType::get(T::getType().type, N) };
    }
};

template <typename T, int N>
struct Vector {
    using type = typename T::type[N];
    static const TypeSize size = N * T::size;
    static Type getType() {
        return Type{ llvm::VectorType::get(T::getType().type, N) };
    }
};

template <typename... Ts>
struct Struct {
    static const TypeSize size = 0; // TODO
    static Type getType() {
        return Type{ llvm::StructType::get(Ts::getType().type) };
    }
};

// Pointer type
template <typename T>
struct Pointer {
    static Type getType() {
        return Type{ llvm::PointerType::get(T::getType().type, 0) };
    }
};

}  // namespace hir
}  // namespace cpu