#pragma once

namespace kiste
{
  enum class command_type
  {
    none,
    cpp,
    text,
    trim_trailing_return,
    escape,
    raw,
    call
  };
}