/*
    This file is meant to handle objects, including special flags, assignment rules, and mutation rules. Due to heavy template use, it is header only.

    Intended instantiation mechanisms:

    Primative (numerical, string, vec)
    - Immediate members
    - Default 0 or empty

    Objects
    - Factory function (static funtions)
    - Procedural (default initializer, use methods)
    - Data-driven (binary, array, or text description of object, i.e. serialized)
    - Prototype ("constructor" function definition)
    - Scope collection (all exported variables in scope implicitly used)
*/
#pragma once

#include <cstdint>
#include <vector>
#include <memory>

namespace CAD{
    namespace language{
        template <class T, template <class> class P>
        class Object{
        public:
            // Modifier flags
            static constexpr uint32_t FLAG_MODIFIER_NONE =          0;          // No flag
            static constexpr uint32_t FLAG_MODIFIER_CONSTANT =      1 <<  0;    // Cannot modify variable (but can rebind)
            static constexpr uint32_t FLAG_MODIFIER_FINAL =         1 <<  1;    // Cannot rebind variable unless undefined (but can modify)
            static constexpr uint32_t FLAG_MODIFIER_EXPORT =        1 <<  2;    // Kept during scope clearing
            // Modifier flag groups
            static constexpr uint32_t FLAGS_MODIFIER_CONSTEXPR =    FLAG_MODIFIER_FINAL | FLAG_MODIFIER_CONSTANT;

            // Data flags
            static constexpr uint32_t FLAG_DATA_NONE =              0;          // No flag
            // Empty data flags
            static constexpr uint32_t FLAG_DATA_NULL =              1 <<  0;    // Explicitly empty (nullptr data)
            static constexpr uint32_t FLAG_DATA_UNDEFINED =         1 <<  1;    // No value given (nullptr data)
            static constexpr uint32_t FLAG_DATA_UNDECLARED =        1 <<  2;    // Returned before declared (nullptr data)
            // Invalid data flags
            static constexpr uint32_t FLAG_DATA_INVALID_OPERANDS =  1 <<  3;    // Operator given empty or invalid operands
            static constexpr uint32_t FLAG_DATA_ERROR =             1 <<  4;    // Function / assignment threw error
            static constexpr uint32_t FLAG_DATA_ASSIGNMENT_ERROR =  1 <<  5;    // Assignment threw error
            static constexpr uint32_t FLAG_DATA_CONSTRUCTOR_ERROR = 1 <<  6;    // Constructor threw error
            // Data flag groups
            static constexpr uint32_t FLAGS_DATA_EMPTY =             FLAG_DATA_NULL | FLAG_DATA_UNDEFINED | FLAG_DATA_UNDECLARED;
            static constexpr uint32_t FLAGS_DATA_ERROR =             FLAG_DATA_INVALID_OPERANDS | FLAG_DATA_ERROR | FLAG_DATA_ASSIGNMENT_ERROR | FLAG_DATA_CONSTRUCTOR_ERROR;
            static constexpr uint32_t FLAGS_DATA_INVALID =           FLAGS_DATA_EMPTY | FLAGS_DATA_ERROR;

            // Assignment flags
            static constexpr uint32_t FLAG_ASSIGNMENT_NONE =                0;          // No flags
            static constexpr uint32_t FLAG_ASSIGNMENT_CLONE =               1 << 17;    // Make deep copy of variable
            static constexpr uint32_t FLAG_ASSIGNMENT_CONDITIONAL_LEFT =    1 << 18;    // Assign only if LHS is invalid
            static constexpr uint32_t FLAG_ASSIGNMENT_CONDITIONAL_RIGHT =   1 << 19;    // Assign only if RHS is valid
            static constexpr uint32_t FLAG_ASSIGNMENT_DECLARE =             1 << 20;    // Allows assignment for forward-declared constexpr

            static constexpr auto IS_PRIMATIVE_TYPE = std::is_arithmetic_v<T> || std::is_same_v<T, std::string>;
        public:
            struct FlaggedValue{
                std::unique_ptr<T> value;
                uint32_t dataFlags;
                FlaggedValue(uint32_t dataFlags = FLAG_DATA_UNDEFINED):value{nullptr},dataFlags{dataFlags}{};
                FlaggedValue(T value, uint32_t dataFlags):value{std::make_unique<T>(value)},dataFlags{dataFlags}{};
                FlaggedValue(std::unique_ptr<T> value, uint32_t dataFlags):value{std::move(value)},dataFlags{dataFlags}{};
                FlaggedValue clone(){
                    if constexpr (IS_PRIMATIVE_TYPE){
                        return FlaggedValue(this->value.get(), this->dataFlags);
                    }else{
                        return FlaggedValue(this->value.clone(), this->dataFlags);
                    }
                }
            };
            uint32_t modifierFlags;
            P<FlaggedValue> data;
        public:
            // Empty object constructor. Used for empty initialization (i.e. no value). Defaults to undefined.
            // template <typename = std::enable_if_t<!std::is_same_v<P<int>,std::weak_ptr<int>>>>
            Object(uint32_t modifierFlags):modifierFlags{modifierFlags},data{std::make_shared<FlaggedValue>()}{

            };
            operator std::string(){
                std::string res;
                if(this->modifierFlags){
                    res += "(";
                }
                if(this->modifierFlags & FLAG_MODIFIER_EXPORT){
                    res += "export ";
                }
                if(this->modifierFlags & FLAG_MODIFIER_CONSTANT){
                    res += "const ";
                }
                if(this->modifierFlags & FLAG_MODIFIER_FINAL){
                    res += "final";
                }
                if(this->modifierFlags){
                    res += ") ";
                }
                if(this->data == nullptr){
                    res += "MISSING";
                }else{
                    uint32_t dataFlags = this->data->dataFlags;
                    T *dataPtr = this->data->value.get();
                    if(dataFlags & FLAG_DATA_NONE){
                        res += "ERROR: Missing data, but no flags";
                    }else if(dataFlags & FLAG_DATA_NULL){
                        res += "NULL";
                    }else if(dataFlags & FLAG_DATA_UNDEFINED){
                        res += "UNDEFINED";
                    }else if(dataFlags & FLAG_DATA_UNDECLARED){
                        res += "UNDECLARED";
                    }else{
                        if constexpr (std::is_same_v<T, std::string>){
                            res += this->data->value.get();
                        } else if constexpr (IS_PRIMATIVE_TYPE){
                            res += std::to_string(*(this->data->value.get()));
                        }else{
                            res += (std::string)*(this->data->value.get());
                        }
                    }
                }

                return res;
            };
            
            bool assign(Object &o, uint32_t assignmentFlags){
                return this->assign(o.data, o.modifierFlags, assignmentFlags);
            }
            // Attempts to assign data to an Object. Returns false if this is not allowed, or true if successful or conditionally skipped.
            // To assign values not from another object, set srcModFlags to FLAG_MODIFIER_NONE.
            bool assign(P<FlaggedValue> data, uint32_t srcModFlags, uint32_t assignmentFlags){
                uint32_t dstDataFlags = this->data->dataFlags;
                uint32_t srcDataFlags = data->dataFlags;
                uint32_t &dstModFlags = this->modifierFlags;

                // Final can only be assigned to if undefined or being declared
                if(dstModFlags & FLAG_MODIFIER_FINAL){
                    if(!(dstDataFlags & FLAG_DATA_UNDEFINED) && !((dstDataFlags & FLAG_DATA_UNDECLARED) && (assignmentFlags & FLAG_ASSIGNMENT_DECLARE))){
                        return false;
                    }
                }
                // Cannot make non-const reference of const value
                if((dstModFlags & FLAG_MODIFIER_CONSTANT) && !(assignmentFlags & FLAG_ASSIGNMENT_CLONE)){
                    return false;
                }
                // Conditional right flag prevent invalid data assignments
                if(assignmentFlags & FLAG_ASSIGNMENT_CONDITIONAL_RIGHT){
                    if(srcDataFlags & FLAGS_DATA_INVALID){
                        return true;
                    }
                }
                // Conditional left flag prevents changing valid data
                if(assignmentFlags & FLAG_ASSIGNMENT_CONDITIONAL_LEFT){
                    if(!(dstDataFlags & FLAGS_DATA_INVALID)){
                        return true;
                    }
                }
                // If src is undeclared, assign invalid
                if(srcDataFlags & FLAG_DATA_UNDECLARED){
                    this->data = std::make_shared<FlaggedValue>(FLAG_DATA_ASSIGNMENT_ERROR);
                    return false;
                }

                // Assign value
                if constexpr (IS_PRIMATIVE_TYPE){
                    // For primative types, use c++ pass by value for copy and clone
                    if(assignmentFlags & (FLAG_ASSIGNMENT_CLONE)){
                        this->data = std::make_shared<FlaggedValue>(*data->value, data->dataFlags);
                    }else{
                        this->data = data;
                    }
                }else{
                    // if(flags & FLAG_ASSIGNMENT_CLONE){
                    //     this->data = o.cloneData();
                    // }else{
                        this->data = data;
                    // }
                }
                return true;
            }
        };
    }
}