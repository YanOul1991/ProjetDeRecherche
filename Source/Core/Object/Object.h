/* ======================================================================================
 *  Object.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Reflection/OptimReflection.h"
#include "Core/Utilities/Random/Random.h"

/**
 * @brief 
 * Base class for all class Types that support reflection.
 */
class CORE_API Object
{
  DECLARE_OBJECT()

 public:
  Object();
  virtual ~Object();

  /**
   * @brief
   * Get a an Object reference by its GUID.
   * 
   * @param guid
   * The GUID of the Object to look for.
   * 
   * @return
   * If found, returns a pointer to the Object,
   * if not returns nullptr.
   */
  static Object* getObject(const SGuid& guid);

  int objectField = 10;

  /**
   * @brief 
   * Get this object inherites from another type object.
   * This only works for classes that inherite from Object hierarchy.
   * 
   * @param type
   * Pointer to the TypeInfo struct of the Object type class to check.
   */
  bool isChildOf(const TypeInfo* type) const;

 protected:
  SGuid m_guid{};
};

/**
 * @brief
 * Prints all the fields and their values of an Object or Structure type instance.
 * 
 * @param object
 * Pointer to the instance.
 * 
 * @param type
 * Pointer to the object's TypeInfo.
 * 
 * @param indent
 * Indentation when printing.
 */
void CORE_API printFields(void* object, const TypeInfo* type, int indent);

void CORE_API printTypeFields(const TypeInfo* type, int indent = 2);