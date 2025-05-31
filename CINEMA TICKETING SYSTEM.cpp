#include <iostream>      // Required for input/output operations
#include <fstream>       // Required for file operations
#include <cstring>       // Required for string operations
#include <iomanip>       // Required for formatted output

using namespace std;     // Use standard namespace

// Constants
const int MAX_MOVIES = 5;        // Maximum number of movies
const int MAX_SEATS = 30;        // Maximum number of seats per movie
const int MAX_CUSTOMERS = 200;   // Maximum number of customers
const int SEATS_PER_ROW = 6;     // Maximum number of seats per row

// Function prototypes
void displayMovies(char movies[MAX_MOVIES][100], int& numMovies);
void bookTicket(char movies[MAX_MOVIES][100], bool seats[MAX_MOVIES][MAX_SEATS], char customerNames[MAX_MOVIES][MAX_SEATS][100], int numMovies, const char* customerName, int movieChoice, int seatNumber);
void displayBookings(char movies[MAX_MOVIES][100], bool seats[MAX_MOVIES][MAX_SEATS], int numMovies, char customerNames[MAX_MOVIES][MAX_SEATS][100]);
void saveBookingsToFile(bool seats[MAX_MOVIES][MAX_SEATS], int numMovies, char customerNames[MAX_MOVIES][MAX_SEATS][100]);
void loadBookingsFromFile(bool seats[MAX_MOVIES][MAX_SEATS], int& numMovies, char customerNames[MAX_MOVIES][MAX_SEATS][100]);
void cancelBooking(char movies[MAX_MOVIES][100], bool seats[MAX_MOVIES][MAX_SEATS], int numMovies, char customerNames[MAX_MOVIES][MAX_SEATS][100], const char* customerName, int movieChoice, int seatNumber);
void processInputFile(const char* inputFile, const char* outputFile, char movies[MAX_MOVIES][100], bool seats[MAX_MOVIES][MAX_SEATS], char customerNames[MAX_MOVIES][MAX_SEATS][100], int& numMovies);

// Main Function
int main() {
    char movies[MAX_MOVIES][100] = { "Inside Out 2", "How To Make Millions Before Grandma Died", "Haikyu !! : The Dumpster Battle", "Bad Boys Ride or Die", "The Garfield Movie" }; // Movie titles
    bool seats[MAX_MOVIES][MAX_SEATS] = { false }; // Seats availability for each movie
    char customerNames[MAX_MOVIES][MAX_SEATS][100] = { "" }; // Customer names for each booked seat
    int numMovies = MAX_MOVIES; // Number of movies
    int choice; // User’s menu choice
    bool returnToMenu; // Flag to return to menu

    loadBookingsFromFile(seats, numMovies, customerNames); // Load existing bookings from file

    do {
        // Display the main menu
        cout << "======================================\n";
        cout << "        Cinema Ticketing System       \n";
        cout << "======================================\n";
        cout << "1. Display Movies\n";
        cout << "2. Display Bookings\n";
        cout << "3. Book Ticket\n";
        cout << "4. Cancel Booking\n";
        cout << "5. Save Bookings to File\n";
        cout << "6. Load Bookings from File\n";
        cout << "7. Process Input File\n";
        cout << "8. Exit\n";
        cout << "--------------------------------------\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            displayMovies(movies, numMovies); // Display available movies
            break;
        case 2:
            displayBookings(movies, seats, numMovies, customerNames); // Display current bookings
            break;
        case 3: {
            char customerName[100];
            int movieChoice, seatNumber;
            cout << "Enter your name: ";
            cin >> customerName;
            displayMovies(movies, numMovies); // Display available movies
            cout << "Enter the number of the movie you want to watch: ";
            cin >> movieChoice;
            cout << "Enter the seat number you want to book (1-" << MAX_SEATS << "): ";
            cin >> seatNumber;
            bookTicket(movies, seats, customerNames, numMovies, customerName, movieChoice, seatNumber); // Book a ticket
            break;
        }
        case 4: {
            char customerName[100];
            int movieChoice, seatNumber;
            cout << "Enter your name: ";
            cin >> customerName;
            displayMovies(movies, numMovies); // Display available movies
            cout << "Enter the number of the movie for which you want to cancel the booking: ";
            cin >> movieChoice;
            cout << "Enter the seat number you want to cancel (1-" << MAX_SEATS << "): ";
            cin >> seatNumber;
            cancelBooking(movies, seats, numMovies, customerNames, customerName, movieChoice, seatNumber); // Cancel a booking
            break;
        }
        case 5:
            saveBookingsToFile(seats, numMovies, customerNames); // Save bookings to file
            break;
        case 6:
            loadBookingsFromFile(seats, numMovies, customerNames); // Load bookings from file
            break;
        case 7: {
            char inputFile[100], outputFile[100];
            cout << "Enter the input file name: ";
            cin >> inputFile;
            cout << "Enter the output file name: ";
            cin >> outputFile;
            processInputFile(inputFile, outputFile, movies, seats, customerNames, numMovies); // Process input file
            break;
        }
        case 8:
            cout << "Exiting the system.\n"; // Exit the system
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n"; // Invalid menu choice
        }

        // Prompt user to return to main menu
        char userResponse;
        cout << "Do you want to return to the main menu? (y/n): ";
        cin >> userResponse;
        returnToMenu = (userResponse == 'y');

    } while (returnToMenu);

    cout << "Exiting the system.\n";
    return 0;
}

// Function to display available movies
void displayMovies(char movies[MAX_MOVIES][100], int& numMovies) {
    char sortChoice;
    cout << "Do you want to sort the movies by name? (y/n): ";
    cin >> sortChoice;

    if (sortChoice == 'y') {
        // Sort movies alphabetically
        for (int i = 0; i < numMovies - 1; i++) {
            for (int j = i + 1; j < numMovies; j++) {
                if (strcmp(movies[i], movies[j]) > 0) {
                    char temp[100];
                    strcpy_s(temp, movies[i]);
                    strcpy_s(movies[i], movies[j]);
                    strcpy_s(movies[j], temp);
                }
            }
        }
        cout << "Movies sorted by name.\n";
    }

    // Display sorted or unsorted movies
    cout << "======================================\n";
    cout << "           Available Movies           \n";
    cout << "======================================\n";
    for (int i = 0; i < numMovies; i++) {
        cout << i + 1 << ". " << movies[i] << endl;
    }
    cout << "======================================\n";
}

// Function to book a ticket
void bookTicket(char movies[MAX_MOVIES][100], bool seats[MAX_MOVIES][MAX_SEATS], char customerNames[MAX_MOVIES][MAX_SEATS][100], int numMovies, const char* customerName, int movieChoice, int seatNumber) {
    if (movieChoice > 0 && movieChoice <= numMovies && seatNumber > 0 && seatNumber <= MAX_SEATS) {
        if (seats[movieChoice - 1][seatNumber - 1]) {
            cout << "Sorry, the seat is already booked.\n"; // Seat is already booked
        }
        else {
            seats[movieChoice - 1][seatNumber - 1] = true; // Mark seat as booked
            strcpy_s(customerNames[movieChoice - 1][seatNumber - 1], customerName); // Store customer name
            cout << "Ticket booked successfully for " << movies[movieChoice - 1] << " at seat number " << seatNumber << ".\n";
        }
    }
    else {
        cout << "Invalid movie or seat number.\n"; // Invalid movie or seat number
    }
}

// Function to cancel a booking
void cancelBooking(char movies[MAX_MOVIES][100], bool seats[MAX_MOVIES][MAX_SEATS], int numMovies, char customerNames[MAX_MOVIES][MAX_SEATS][100], const char* customerName, int movieChoice, int seatNumber) {
    if (movieChoice > 0 && movieChoice <= numMovies && seatNumber > 0 && seatNumber <= MAX_SEATS) {
        if (seats[movieChoice - 1][seatNumber - 1] && strcmp(customerNames[movieChoice - 1][seatNumber - 1], customerName) == 0) {
            seats[movieChoice - 1][seatNumber - 1] = false; // Mark seat as available
            customerNames[movieChoice - 1][seatNumber - 1][0] = '\0'; // Clear the customer name
            cout << "Booking canceled successfully for " << movies[movieChoice - 1] << " at seat number " << seatNumber << ".\n";
        }
        else {
            cout << "The seat is either already available or booked by someone else.\n";
        }
    }
    else {
        cout << "Invalid movie or seat number.\n";
    }
}

// Function to display bookings
void displayBookings(char movies[MAX_MOVIES][100], bool seats[MAX_MOVIES][MAX_SEATS], int numMovies, char customerNames[MAX_MOVIES][MAX_SEATS][100]) {
    const int ROWS = MAX_SEATS / SEATS_PER_ROW;
    for (int i = 0; i < numMovies; i++) {
        cout << "======================================\n";
        cout << "     Bookings for Movie: " << i + 1 << " - " << movies[i] << "\n";
        cout << "======================================\n";
        cout << "Seat Layout:\n";
        cout << "======================================\n";

        for (int row = 0; row < ROWS; row++) {
            cout << "Row " << row + 1 << ":\t";
            for (int col = 0; col < SEATS_PER_ROW; col++) {
                int seatNumber = row * SEATS_PER_ROW + col;
                if (seats[i][seatNumber]) {
                    cout << setw(10) << left << customerNames[i][seatNumber]; // Display customer name if booked
                }
                else {
                    cout << "O          "; // Display 'O' for available seat
                }
            }
            cout << "\n";
        }
        cout << "======================================\n";
        cout << "Hint: O - Available, customer name - Booked\n";
        cout << "======================================\n";
    }
}

// Function to save bookings to file
void saveBookingsToFile(bool seats[MAX_MOVIES][MAX_SEATS], int numMovies, char customerNames[MAX_MOVIES][MAX_SEATS][100]) {
    ofstream outFile("bookings.txt");
    for (int i = 0; i < numMovies; i++) {
        for (int j = 0; j < MAX_SEATS; j++) {
            outFile << seats[i][j] << " " << customerNames[i][j] << " ";
        }
        outFile << endl;
    }
    outFile.close();
    cout << "Bookings saved to file.\n";
}

// Function to load bookings from file
void loadBookingsFromFile(bool seats[MAX_MOVIES][MAX_SEATS], int& numMovies, char customerNames[MAX_MOVIES][MAX_SEATS][100]) {
    ifstream inFile("bookings.txt");
    if (inFile.is_open()) {
        for (int i = 0; i < numMovies; i++) {
            for (int j = 0; j < MAX_SEATS; j++) {
                inFile >> seats[i][j] >> customerNames[i][j];
            }
        }
        inFile.close();
        cout << "Bookings loaded from file.\n";
    }
    else {
        cout << "No existing bookings file found.\n";
    }
}

// Function to process input file and generate output file with improved formatting
void processInputFile(const char* inputFile, const char* outputFile, char movies[MAX_MOVIES][100], bool seats[MAX_MOVIES][MAX_SEATS], char customerNames[MAX_MOVIES][MAX_SEATS][100], int& numMovies) {
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);

    if (inFile.is_open()) {
        char command[10];
        while (inFile >> command) {
            if (strcmp(command, "BOOK") == 0) {
                char customerName[100];
                int movieChoice, seatNumber;
                inFile >> customerName >> movieChoice >> seatNumber;
                bookTicket(movies, seats, customerNames, numMovies, customerName, movieChoice, seatNumber); // Book ticket from input file
            }
            else if (strcmp(command, "CANCEL") == 0) {
                char customerName[100];
                int movieChoice, seatNumber;
                inFile >> customerName >> movieChoice >> seatNumber;
                cancelBooking(movies, seats, numMovies, customerNames, customerName, movieChoice, seatNumber); // Cancel booking from input file
            }
        }
        inFile.close();

        // Write the final bookings state to the output file with improved formatting
        const int ROWS = MAX_SEATS / SEATS_PER_ROW;
        for (int i = 0; i < numMovies; i++) {
            outFile << "======================================\n";
            outFile << "     Bookings for Movie: " << i + 1 << " - " << movies[i] << "\n";
            outFile << "======================================\n";
            outFile << "Seat Layout:\n";
            outFile << "======================================\n";

            for (int row = 0; row < ROWS; row++) {
                outFile << "Row " << row + 1 << ":\t";
                for (int col = 0; col < SEATS_PER_ROW; col++) {
                    int seatNumber = row * SEATS_PER_ROW + col;
                    if (seats[i][seatNumber]) {
                        outFile << setw(10) << left << customerNames[i][seatNumber]; // Display customer name if booked
                    }
                    else {
                        outFile << "O          "; // Display 'O' for available seat
                    }
                }
                outFile << "\n";
            }
            outFile << "======================================\n";
            outFile << "Hint: O - Available, customer name - Booked\n";
            outFile << "======================================\n";
        }
        outFile.close();
        cout << "Processed input file and generated output file.\n";
    }
    else {
        cout << "Unable to open input file.\n";
    }
}