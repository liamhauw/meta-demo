/*
  Copyright (C) 2023-present Liam Hauw.

  SPDX license identifier: MIT.

  Reflection register header file.
*/

#pragma once

namespace luka {

namespace reflection {

class TypeMetaRegister {
 public:
  static void MetaRegister();
  static void MetaUnregister();
};

}  // namespace reflection

}  // namespace luka