// Serializer.cpp

#include "Core/Serialization/Serializer.h"

#include "Core/Reflection/OptimReflection.h"

#include <fstream>
#include <iostream>
#include <string>

static void serializeFields(std::ofstream& out, void* param_pObject, const TypeInfo* param_typeInfo, const FieldInfo* param_fieldInfo) {

}

/**
 * @brief
 * Serialized an object by its type.
 */
static void serializeObject(std::ofstream& out, void* param_pObject, const TypeInfo* param_typeInfo, int param_indent = 0) {
  std::string _indent(param_indent * 2, ' ');
  uint64 index = 0;

  switch (param_typeInfo->typeData) {
  case TypeData::Primitive:
    out << param_typeInfo->toString(param_pObject);
    break;

  case TypeData::Object:
  case TypeData::Structure:
    out << "(\n";
    for (auto& fieldInfo : param_typeInfo->fields) {
      void* pField = (char*) param_pObject + fieldInfo.offset;

      out << _indent << fieldInfo.name << "=";

      serializeObject(out, pField, fieldInfo.typeInfo, param_indent + 1);

      if (++index < param_typeInfo->fields.size()) {
        out << ",\n";
      }
    }
    out << "\n" << _indent << ")";
    break;

  default:
    break;
  }
}

bool Serializer::SaveScene(const std::vector<Object*>& objects, const char* sceneName) {
  std::string saveLocation;

  saveLocation += "Scenes/";
  saveLocation += sceneName;
  saveLocation += ".oescene";

  try {
    std::ofstream out(saveLocation);

    if (!out.is_open()) {
      std::string _errMessage = "Cannot open file at: ";
      _errMessage + saveLocation;
      throw std::exception(_errMessage.c_str());
    }

    //std::cout << "Trying to save scene at: " << saveLocation << '\n';
    //outputStream << "This is a test for saving a new scene!\n";

    int index = 0;

    for (auto& obj : objects) {
      out << obj->GetTypeInfo()->name << "=";
      serializeObject(out, obj, obj->GetTypeInfo(), 1);

      if (++index < objects.size()) {
        out << ",\n";
      }
    }

    out.close();

    return true;
  }
  catch (const std::exception& e) {
    std::cout << "[Serializer - Exception]" << e.what() << '\n';
    return false;
  }
}
