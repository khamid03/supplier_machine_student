
# Vending Machine Simulation with Supplier, Machine, and Student

This project simulates a vending machine system involving three primary actors:
1. **Machine** - Dispenses drinks based on coin insertion.
2. **Student** - Inserts coins into the machine and retrieves drinks.
3. **Supplier** - Loads drinks into the machine and collects coins.

The program utilizes semaphores to synchronize access to shared resources, ensuring proper coordination between the actors.

## Problem Description

The vending machine requires proper synchronization for:
- **Coins**: Inserted by the student for dispensing drinks.
- **Drink Supply**: Managed by the supplier who loads the machine when empty.

Each actor in the system has specific responsibilities:
- **Machine**: Dispenses drinks if there are coins and drinks available.
- **Student**: Inserts coins, signaling the machine to dispense a drink.
- **Supplier**: Loads drinks into the machine when it’s empty and collects coins.

## Key Semaphores

The code uses the following semaphores for synchronization:
- `empty`: Signals when the machine is empty and needs more drinks.
- `full`: Tracks the availability of drinks in the machine.
- `available`: Ensures mutual exclusion, giving one actor access to the machine at a time.
- `all_coins`: Tracks if enough coins have been inserted for a drink to be dispensed.

## Code Structure

### Functions Overview

1. **Machine Function**  
   The machine function waits for a signal indicating that drinks are available (`full`). Once drinks are available and coins are inserted (`all_coins`), the machine dispenses a drink.

2. **Student Function**  
   The student function waits for the machine to be available (`available`), inserts coins, and signals the machine that enough coins are present (`all_coins`). After a drink is dispensed, the student picks it up and signals availability.

3. **Supplier Function**  
   The supplier function waits until the machine is empty (`empty`) and loads new drinks, updating the `full` semaphore to indicate the available drinks. The supplier also collects coins before signaling that the machine is available.

## Compilation and Execution

1. **Compile the code** using a C compiler:
   ```bash
   gcc -o vending_machine_simulation supplier_and_machine_and_student.c -lpthread
   ```

2. **Run the program** with optional command-line parameters:
   ```bash
   ./vending_machine_simulation [-n num_students] [-c capacity]
   ```

   - **`-n <num_students>`**: Sets the number of students in the simulation (default is `2`).
   - **`-c <capacity>`**: Sets the machine’s drink capacity (default is `4`).

## Pseudocode

The pseudocode for this simulation is detailed in the `Pseudocode.txt` file. It provides a high-level view of each function’s logic, illustrating how semaphores are used to coordinate between the machine, student, and supplier.

## License

This project is open-source and available under the MIT License.
