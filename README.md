# FactoryRegister
An implementation of factory pattern in C++17 using templates and smart pointers.
Example usage:

```
#include "FactoryRegister.h"
#include <iostream>
#include <string>

class Person {
public:
    Person(std::string name, int age) : name_(name), age_(age) {}
    std::string name() const { return name_; }
    int age() const { return age_; }
private:
    std::string name_;
    int age_;
};

int main() {
    // Create an instance of FactoryRegister with appropriate template arguments
    Utils::FactoryRegister<Person, std::string> personRegistry;

    // Register instances of the Person class with the registry
    personRegistry.add("John", []() { return std::make_unique<Person>("John Doe", 35); });
    personRegistry.add("Jane", []() { return std::make_unique<Person>("Jane Doe", 13); }); // use a lambda to create instance

    // Create instances of the Person class using the registry
    auto john = personRegistry.generate("John");
    auto jane = personRegistry.generate("Jane");

    // Use the created instances
    std::cout << john.value()->name() << " is " << john.value()->age() << " years old.\n";
    std::cout << jane.value()->name() << " is " << jane.value()->age() << " years old.\n";

    return 0;
}

```

Output:

```
John Doe is 35 years old.
Jane Doe is 13 years old.
```
