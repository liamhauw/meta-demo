/*
  SPDX license identifier: MIT

  Copyright (C) 2023-present Liam Hauw.

  Reflection register header file.
*/

#pragma once

namespace luka {

namespace Reflection {

class TypeMetaRegister {
 public:
  static void metaRegister();
  static void metaUnregister();
};

}  // namespace Reflection

}  // namespace luka