# kiss-templates
Type safe _"Keep it simple, stupid"_ text templates for C++

## How it works:
Use kiste2cpp to turn text templates into type- and name-safe C++ code that generates text from your data.

### How templates look like:
Template are a mix of
  - kiss template commands (there are VERY few)
  - C++
  - text

```
%namespace test
%{
  $class Hello

  %auto render() const -> void
  %{
    Hello ${data.name}!
  %}

  $endclass
%}
```

### Generating C++ code:
I bet you can guess what this all means (and it is documented below), so let's compile this into a C++ header file:

```
kiste2cpp hello_world.kt > hello_world.h
```

### Using the generated code:
And now we use it in our C++ project like this

```C++
#include <iostream>
#include <hello_world.h>
#include <kiste/raw.h>

struct Data
{
  std::string name;
};

int main()
{
  const auto data = Data{"World"};
  auto& os = std::cout;
  const auto serializer = kiste::raw{os};
  auto hello = test::Hello(data, serializer);

  hello.render();
}
```

### Output:
Compile and run:
```
$ ./examples/0_hello_world/hello_world 
    Hello World!
```
Yeah!

## The kiss template syntax

### C++
Any line that starts with zero or more spaces and a `%` is interpreted as C++.
For example
```
%#include<string>
%namespace
%{
    %auto foo() -> std::string
    %{
      return "bar";
    %}
%}
```
There is really nothing to it, just a `%` at the beginning of the line

### Template classes
All text of the template is located in functions of template classes. Template classes start with `$class <name> [: <base>` and end with `$endclass`.

This is a stand-alone class:
```
$class base
% // Some stuff
$endclass
```
And this is a derived class
```
%#include <base.h>
$class derived : base
% // Some other stuff
$endclass
```
In the generated code, the parent will also be the base of the child. They are linked in such a way that 

  - you can access the direct child via a `child` member variable in the parent
  - you can access anything inherited from parent, grandparent, etc via a `parent` member

### Serializing data
As you saw in the initial example, the generated template code is initialized with data and a serializer. You can serialize members of that data or in fact any C++ expression by enclosing it in `${}`. For instance

```
%for (const auto& entry : data.entries)
%{
  First name: ${entry.first}
  Last name: ${entry.last}
  Size of names: ${entry.first.size() + entry.last.size()}
%}
```

### Calling functions:
If you want to call a function without serializing the result (e.g. because the function returns `void`), you can enclose the call in `$call{}`.

### Trimming:
  - left-trim of a line: Zero or more spaces/tabs followed by `$|`
  - right-trim of a line (including the trailing return): `$|` at the end of the line

For example:
```
%auto title() const -> void
%{
   $| Hello ${data.name}! $|
%}
```
This will get rid of the leading spaces and the trailing return, yielding something like

```
 Hello Mr. Wolf! 
```

### Escape sequences:
  - `$$` -> `$`
  - `$%` -> `%`

### Text:
Text is everything else, as long as it is inside a function of a template class.

## Further education
This is pretty much it.

There are several examples in the `examples` folder. If you have questions, please do not hesitate to open an issue.
