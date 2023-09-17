#include <iostream>
#include "MyPtrLib.h"

int main() 
{
   //UNIQUEPTR TESTS!!
    UniquePtr<int> ptr1(new int(1488));
    UniquePtr<int> ptr2;

    std::cout << "ptr1: " << *ptr1 << std::endl; // Should print "ptr1: 1488"
    std::cout << "ptr2: " << (ptr2 ? *ptr2 : 0) << std::endl; // Should print "ptr2: 0"

    ptr2 = std::move(ptr1);

    std::cout << "ptr1: " << (ptr1 ? *ptr1 : 0) << std::endl; // Should print "ptr1: 0"
    std::cout << "ptr2: " << *ptr2 << std::endl; // Should print "ptr2: 1488"

    UniquePtr<int> ptr3 = std::move(ptr2);

    std::cout << "ptr2: " << (ptr2 ? *ptr2 : 0) << std::endl; // Should print "ptr2: 0"
    std::cout << "ptr3: " << *ptr3 << std::endl; // Should print "ptr3: 1488"

    ptr3.reset();

    std::cout << "ptr3 after reset: " << (ptr3 ? *ptr3 : 0) << std::endl; // Should print "ptr3 after reset: 0"

    //дальше не тестил

    // Create a SharedPtr and test its basic functionality
    SharedPtr<int> sharedPtr1(new int(42));
    SharedPtr<int> sharedPtr2 = sharedPtr1;

    std::cout << "sharedPtr1 count: " << sharedPtr1.useCount() << std::endl; // Should print "sharedPtr1 count: 2"
    std::cout << "sharedPtr2 count: " << sharedPtr2.useCount() << std::endl; // Should print "sharedPtr2 count: 2"

    // Create a WeakPtr and test its basic functionality
    WeakPtr<int> weakPtr1 = sharedPtr1;
    WeakPtr<int> weakPtr2 = sharedPtr2;

    std::cout << "sharedPtr1 count: " << sharedPtr1.useCount() << std::endl; // Should print "sharedPtr1 count: 4"
    std::cout << "sharedPtr2 count: " << sharedPtr2.useCount() << std::endl; // Should print "sharedPtr2 count: 4"

    // Lock a WeakPtr to get a SharedPtr and access the object
    SharedPtr<int> lockedPtr1 = weakPtr1.lock();
    SharedPtr<int> lockedPtr2 = weakPtr2.lock();

    std::cout << "lockedPtr1 count: " << lockedPtr1.useCount() << std::endl; // Should print "lockedPtr1 count: 4"
    std::cout << "lockedPtr2 count: " << lockedPtr2.useCount() << std::endl; // Should print "lockedPtr2 count: 4"

    // Reset SharedPtr instances
    sharedPtr1.reset();
    sharedPtr2.reset();

    // The object should still exist due to the locked SharedPtr instances
    std::cout << "lockedPtr1 count: " << lockedPtr1.useCount() << std::endl; // Should print "lockedPtr1 count: 2"
    std::cout << "lockedPtr2 count: " << lockedPtr2.useCount() << std::endl; // Should print "lockedPtr2 count: 2"

    // Reset all pointers
    lockedPtr1.reset();
    lockedPtr2.reset();
    weakPtr1.reset();
    weakPtr2.reset();

    // The object should be destroyed now
    std::cout << "Object destroyed" << std::endl;
    return 0;
}

//ui
// замерить с обычными, с одними и теми же данными, попарно одинаковые, но внутри одного указателя рандомные 