# Client lifecycle

<!-- TODO: update client run logic doc -->

What is inside `fj_client_run()`?

1. Allocate client core data
2. Initialise client core data
3. Initialise modules:
    1. Check if the user's module listener is set
    2. Allocate module data
    3. Initialise module data
    4. Call the user's module listener
4. Initialise the user
5. Run event loop:
   1. Wait for messages according to the schedule
   2. Handle messages
   3. Call the user's event loop iteration callback
6. Deinitialise the user
7. Deinitialise modules
   1. Deinitialise module data
   2. Deallocate module data
8. Deinitialise client core data
9. Deallocate client core data

That's quite a lot, isn't it?
