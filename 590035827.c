/*
 * ============================================================================
 * Project Name : Student Record and Result Management System
 * Phase        : Phase 7 - Testing, Debugging & Complete System Polish
 * File Name    : 590035827.c
 * Author       : Manav Rawat (SAP ID: 590035827)
 * Course       : First-Year B.Tech Computer Science & Engineering (CSE)
 * Description  : A robust, fully tested, console-based Student Record and Result
 *                Management System in Standard C. Incorporates all features from
 *                Phases 1 through 6 with comprehensive input validation, bounds
 *                checking, and edge case resilience.
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 100
#define TOTAL_SEMESTERS 8

/* ----------------------------------------------------------------------------
 * Data Structure: struct Student
 * Models academic records and contact details for a student.
 * ---------------------------------------------------------------------------- */
struct Student {
    long long int sap_id;          // Unique 9-digit SAP ID (e.g., 590035827)
    int roll_number;               // University / Class Roll Number
    char name[100];                // Student Full Name
    char phone_number[15];         // Contact Phone Number (e.g., "+91-9876543210")
    char email_id[100];            // Student Email Address
    int year;                      // Current Academic Year of Study (1 to 4)
    int semester;                  // Current Semester of Study (1 to 8)
    float sgpa[TOTAL_SEMESTERS];   // SGPA array for 8 semesters (-1 = unentered)
};

/* ----------------------------------------------------------------------------
 * Function Prototypes
 * ---------------------------------------------------------------------------- */
void clearInputBuffer(void);
float computeStudentCGPA(const struct Student *s, int *valid_semesters);
void addStudent(struct Student students[], int *student_count);
void searchStudent(const struct Student students[], int student_count);
void calculateCGPA(const struct Student students[], int student_count);
void averageCGPAByYear(const struct Student students[], int student_count);
void displayAllStudents(const struct Student students[], int student_count);
void deleteStudent(struct Student students[], int *student_count);
void sortBySAPID(struct Student students[], int student_count);
void sortByName(struct Student students[], int student_count);
void academicAdvisor(const struct Student students[], int student_count);

/* ----------------------------------------------------------------------------
 * Main Function: Interactive Loop and Complete Menu Integration
 * ---------------------------------------------------------------------------- */
int main(void) {
    // Array to store up to 100 students
    struct Student students[MAX_STUDENTS];
    // Counter variable to track the number of students currently stored
    int student_count = 0;
    int choice = 0;

    // Initialize all semester SGPA values in the array to -1 (sentinel)
    for (int i = 0; i < MAX_STUDENTS; i++) {
        for (int j = 0; j < TOTAL_SEMESTERS; j++) {
            students[i].sgpa[j] = -1.0f;
        }
    }

    /* Welcome Banner */
    printf("============================================================\n");
    printf("        STUDENT RECORD AND RESULT MANAGEMENT SYSTEM         \n");
    printf("============================================================\n");
    printf(" Welcome to the College Student Record Management System!   \n");
    printf(" Developed for First-Year B.Tech CSE Project.               \n");
    printf(" Author: Manav Rawat (SAP ID: 590035827)                    \n");
    printf("============================================================\n\n");

    /* Interactive Menu Loop */
    while (1) {
        printf("\n----------------------- MAIN MENU --------------------------\n");
        printf("1.  Add Student\n");
        printf("2.  Search Student\n");
        printf("3.  Calculate CGPA\n");
        printf("4.  Average CGPA by Year\n");
        printf("5.  Display All Students\n");
        printf("6.  Delete Student\n");
        printf("7.  Sort by SAP ID\n");
        printf("8.  Sort by Name\n");
        printf("9.  Academic Advisor\n");
        printf("10. Exit\n");
        printf("------------------------------------------------------------\n");
        printf("Enter your choice (1-10): ");

        // Input validation for menu choice
        if (scanf("%d", &choice) != 1) {
            printf("\n[Error] Invalid input! Please enter a number between 1 and 10.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer(); // Consume newline after choice

        switch (choice) {
            case 1:
                addStudent(students, &student_count);
                break;

            case 2:
                searchStudent(students, student_count);
                break;

            case 3:
                calculateCGPA(students, student_count);
                break;

            case 4:
                averageCGPAByYear(students, student_count);
                break;

            case 5:
                displayAllStudents(students, student_count);
                break;

            case 6:
                deleteStudent(students, &student_count);
                break;

            case 7:
                sortBySAPID(students, student_count);
                break;

            case 8:
                sortByName(students, student_count);
                break;

            case 9:
                academicAdvisor(students, student_count);
                break;

            case 10:
                printf("\n============================================================\n");
                printf(" Thank you for using the Student Record & Result System!\n");
                printf(" Exiting application. Have a wonderful day!\n");
                printf("============================================================\n");
                return 0;

            default:
                printf("\n[Warning] Invalid choice! Please select an option from 1 to 10.\n");
                break;
        }
    }

    return 0;
}

/* ----------------------------------------------------------------------------
 * Helper Function: clearInputBuffer()
 * Flushes any remaining characters from stdin to prevent input bleed.
 * ---------------------------------------------------------------------------- */
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* ----------------------------------------------------------------------------
 * Helper Function: computeStudentCGPA()
 * Calculates the CGPA for a given student strictly from available SGPAs (>= 0).
 * Any semester with SGPA == -1 is ignored.
 * Returns -1.0f if the student has no available SGPAs.
 * ---------------------------------------------------------------------------- */
float computeStudentCGPA(const struct Student *s, int *valid_semesters) {
    float sum = 0.0f;
    int count = 0;

    for (int i = 0; i < TOTAL_SEMESTERS; i++) {
        // Skip unentered / pending semesters marked with -1
        if (s->sgpa[i] >= 0.0f) {
            sum += s->sgpa[i];
            count++;
        }
    }

    if (valid_semesters != NULL) {
        *valid_semesters = count;
    }

    if (count == 0) {
        return -1.0f; // Indicates no valid SGPAs are available
    }

    return sum / (float)count;
}

/* ----------------------------------------------------------------------------
 * Core Function 1: addStudent()
 * Adds a new student record into the array after performing necessary checks:
 * - Checks if maximum capacity (100) is reached.
 * - Validates positive SAP ID and prevents duplicates.
 * - Validates Year (1-4) and Semester (1-8).
 * - Validates SGPAs (0.00-10.00 or -1 for unentered/pending semesters).
 * ---------------------------------------------------------------------------- */
void addStudent(struct Student students[], int *student_count) {
    printf("\n--- [1] Add New Student ---\n");

    if (*student_count >= MAX_STUDENTS) {
        printf("[Error] Storage limit reached (%d students). Cannot add more.\n", MAX_STUDENTS);
        return;
    }

    long long int temp_sap;
    printf("Enter SAP ID (e.g., 590035827): ");
    while (scanf("%lld", &temp_sap) != 1 || temp_sap <= 0) {
        printf("Invalid SAP ID! Please enter a positive integer: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    // Check for duplicate SAP ID
    for (int i = 0; i < *student_count; i++) {
        if (students[i].sap_id == temp_sap) {
            printf("[Error] A student with SAP ID %lld already exists! Insertion rejected.\n", temp_sap);
            return;
        }
    }

    // Initialize all 8 semester SGPAs to -1 for the new student
    for (int j = 0; j < TOTAL_SEMESTERS; j++) {
        students[*student_count].sgpa[j] = -1.0f;
    }

    students[*student_count].sap_id = temp_sap;

    printf("Enter Roll Number: ");
    while (scanf("%d", &students[*student_count].roll_number) != 1 ||
           students[*student_count].roll_number <= 0) {
        printf("Invalid Roll Number! Enter a positive integer: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    printf("Enter Full Name: ");
    if (scanf(" %99[^\n]", students[*student_count].name) != 1) {
        strcpy(students[*student_count].name, "Unknown");
    }
    clearInputBuffer();

    printf("Enter Phone Number: ");
    if (scanf("%14s", students[*student_count].phone_number) != 1) {
        strcpy(students[*student_count].phone_number, "N/A");
    }
    clearInputBuffer();

    printf("Enter Email ID: ");
    if (scanf("%99s", students[*student_count].email_id) != 1) {
        strcpy(students[*student_count].email_id, "N/A");
    }
    clearInputBuffer();

    printf("Enter Year of Study (1-4): ");
    while (scanf("%d", &students[*student_count].year) != 1 ||
           students[*student_count].year < 1 || students[*student_count].year > 4) {
        printf("Invalid Year! Please enter an integer between 1 and 4: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    printf("Enter Semester of Study (1-8): ");
    while (scanf("%d", &students[*student_count].semester) != 1 ||
           students[*student_count].semester < 1 || students[*student_count].semester > 8) {
        printf("Invalid Semester! Please enter an integer between 1 and 8: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    printf("\n--- SGPA Entry ---\n");
    printf("Enter SGPA for completed semesters (0.00 to 10.00, or enter -1 if pending):\n");
    for (int s = 0; s < students[*student_count].semester; s++) {
        float temp_sgpa;
        while (1) {
            printf("  Semester %d SGPA (-1 if pending): ", s + 1);
            if (scanf("%f", &temp_sgpa) == 1 &&
                ((temp_sgpa >= 0.0f && temp_sgpa <= 10.0f) || temp_sgpa == -1.0f)) {
                students[*student_count].sgpa[s] = temp_sgpa;
                break;
            }
            printf("  [Invalid SGPA] Must be between 0.00 and 10.00, or -1.\n");
            clearInputBuffer();
        }
        clearInputBuffer();
    }

    (*student_count)++;
    printf("\n[Success] Student record added successfully! (Total Students: %d)\n", *student_count);
}

/* ----------------------------------------------------------------------------
 * Core Function 2: searchStudent()
 * Searches for a student by their unique SAP ID using linear search.
 * Displays full student details if found, or 'Student Not Found' message.
 * ---------------------------------------------------------------------------- */
void searchStudent(const struct Student students[], int student_count) {
    printf("\n--- [2] Search Student ---\n");

    if (student_count == 0) {
        printf("[Notice] No student records available to search.\n");
        return;
    }

    long long int target_sap;
    printf("Enter SAP ID to search: ");
    if (scanf("%lld", &target_sap) != 1 || target_sap <= 0) {
        printf("[Error] Invalid SAP ID format.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int found_index = -1;
    for (int i = 0; i < student_count; i++) {
        if (students[i].sap_id == target_sap) {
            found_index = i;
            break;
        }
    }

    if (found_index != -1) {
        int valid_semesters = 0;
        float cgpa = computeStudentCGPA(&students[found_index], &valid_semesters);

        printf("\n==================== STUDENT RECORD FOUND ====================\n");
        printf("SAP ID        : %lld\n", students[found_index].sap_id);
        printf("Roll Number   : %d\n", students[found_index].roll_number);
        printf("Name          : %s\n", students[found_index].name);
        printf("Phone Number  : %s\n", students[found_index].phone_number);
        printf("Email ID      : %s\n", students[found_index].email_id);
        printf("Year of Study : %d\n", students[found_index].year);
        printf("Semester      : %d\n", students[found_index].semester);
        if (valid_semesters > 0) {
            printf("Current CGPA  : %.2f / 10.00 (%d sem evaluated)\n", cgpa, valid_semesters);
        } else {
            printf("Current CGPA  : N/A (No SGPAs entered yet)\n");
        }
        printf("SGPA Details  :\n");
        for (int j = 0; j < TOTAL_SEMESTERS; j++) {
            if (students[found_index].sgpa[j] < 0.0f) {
                printf("  Semester %d : Not Entered / Pending\n", j + 1);
            } else {
                printf("  Semester %d : %.2f\n", j + 1, students[found_index].sgpa[j]);
            }
        }
        printf("==============================================================\n");
    } else {
        printf("\n[Notice] Student Not Found: No record matches SAP ID %lld.\n", target_sap);
    }
}

/* ----------------------------------------------------------------------------
 * Core Function 3: displayAllStudents()
 * Iterates through all stored students and prints their details in a readable format.
 * ---------------------------------------------------------------------------- */
void displayAllStudents(const struct Student students[], int student_count) {
    printf("\n--- [5] Display All Students ---\n");

    if (student_count == 0) {
        printf("[Notice] No student records found. Add students using Option 1.\n");
        return;
    }

    printf("Total Enrolled Students: %d / %d\n", student_count, MAX_STUDENTS);
    printf("=========================================================================================\n");

    for (int i = 0; i < student_count; i++) {
        int valid_sems = 0;
        float cgpa = computeStudentCGPA(&students[i], &valid_sems);

        printf("[%d] SAP ID: %-12lld | Roll No: %-6d | Name: %s\n",
               i + 1, students[i].sap_id, students[i].roll_number, students[i].name);

        if (valid_sems > 0) {
            printf("    Year: %d | Semester: %d | CGPA: %-5.2f | Phone: %s | Email: %s\n",
                   students[i].year, students[i].semester, cgpa, students[i].phone_number, students[i].email_id);
        } else {
            printf("    Year: %d | Semester: %d | CGPA: N/A   | Phone: %s | Email: %s\n",
                   students[i].year, students[i].semester, students[i].phone_number, students[i].email_id);
        }

        printf("    SGPA: [");
        for (int j = 0; j < TOTAL_SEMESTERS; j++) {
            if (students[i].sgpa[j] < 0.0f) {
                printf("S%d: N/A", j + 1);
            } else {
                printf("S%d: %.2f", j + 1, students[i].sgpa[j]);
            }
            if (j < TOTAL_SEMESTERS - 1) printf(", ");
        }
        printf("]\n");
        printf("-----------------------------------------------------------------------------------------\n");
    }
}

/* ----------------------------------------------------------------------------
 * Core Function 4: deleteStudent()
 * Deletes student by SAP ID with array shifting, updating student count cleanly.
 * ---------------------------------------------------------------------------- */
void deleteStudent(struct Student students[], int *student_count) {
    printf("\n--- [6] Delete Student ---\n");

    if (*student_count == 0) {
        printf("[Notice] No student records available to delete.\n");
        return;
    }

    long long int target_sap;
    printf("Enter SAP ID of student to delete: ");
    if (scanf("%lld", &target_sap) != 1 || target_sap <= 0) {
        printf("[Error] Invalid SAP ID format.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int found_index = -1;
    for (int i = 0; i < *student_count; i++) {
        if (students[i].sap_id == target_sap) {
            found_index = i;
            break;
        }
    }

    if (found_index == -1) {
        printf("\n[Notice] Student Not Found: No record matches SAP ID %lld.\n", target_sap);
        return;
    }

    printf("Are you sure you want to delete record for '%s' (SAP ID: %lld)? (y/n): ",
           students[found_index].name, students[found_index].sap_id);
    char confirm = 'n';
    if (scanf(" %c", &confirm) != 1) {
        confirm = 'n';
    }
    clearInputBuffer();

    if (confirm == 'y' || confirm == 'Y') {
        // Shift remaining array elements to overwrite deleted student
        for (int i = found_index; i < *student_count - 1; i++) {
            students[i] = students[i + 1];
        }

        // Clean out stale data at the vacated end slot
        (*student_count)--;
        students[*student_count].sap_id = 0;
        students[*student_count].roll_number = 0;
        for (int j = 0; j < TOTAL_SEMESTERS; j++) {
            students[*student_count].sgpa[j] = -1.0f;
        }

        printf("\n[Success] Student record deleted successfully. Current count: %d\n", *student_count);
    } else {
        printf("\n[Cancelled] Deletion cancelled by user.\n");
    }
}

/* ----------------------------------------------------------------------------
 * Core Function 5: calculateCGPA()
 * Searches for a student by SAP ID, computes CGPA from available SGPAs only.
 * ---------------------------------------------------------------------------- */
void calculateCGPA(const struct Student students[], int student_count) {
    printf("\n--- [3] Calculate CGPA ---\n");

    if (student_count == 0) {
        printf("[Notice] No student records available. Please add students first.\n");
        return;
    }

    long long int target_sap;
    printf("Enter SAP ID of the student: ");
    if (scanf("%lld", &target_sap) != 1 || target_sap <= 0) {
        printf("[Error] Invalid SAP ID format.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int found_index = -1;
    for (int i = 0; i < student_count; i++) {
        if (students[i].sap_id == target_sap) {
            found_index = i;
            break;
        }
    }

    if (found_index == -1) {
        printf("\n[Notice] Student Not Found: No record matches SAP ID %lld.\n", target_sap);
        return;
    }

    int valid_semesters = 0;
    float cgpa = computeStudentCGPA(&students[found_index], &valid_semesters);

    printf("\n==================== CGPA CALCULATION RESULT ====================\n");
    printf("Student Name        : %s\n", students[found_index].name);
    printf("SAP ID              : %lld\n", students[found_index].sap_id);
    printf("Roll Number         : %d\n", students[found_index].roll_number);
    printf("Year of Study       : %d\n", students[found_index].year);
    printf("Current Semester    : %d\n", students[found_index].semester);
    printf("Available Semesters : %d of %d\n", valid_semesters, TOTAL_SEMESTERS);

    if (valid_semesters == 0) {
        printf("Calculated CGPA     : N/A (No semester SGPA values entered yet)\n");
    } else {
        printf("Calculated CGPA     : %.2f / 10.00\n", cgpa);
    }
    printf("=================================================================\n");
}

/* ----------------------------------------------------------------------------
 * Core Function 6: averageCGPAByYear()
 * Computes and displays the average CGPA for each academic year (1 to 4).
 * For each year, only students with at least one valid SGPA are included.
 * ---------------------------------------------------------------------------- */
void averageCGPAByYear(const struct Student students[], int student_count) {
    printf("\n--- [4] Average CGPA by Year ---\n");

    if (student_count == 0) {
        printf("[Notice] No student records available. Please add students first.\n");
        return;
    }

    printf("=========================================================================================\n");
    printf("                             AVERAGE CGPA BREAKDOWN BY YEAR                              \n");
    printf("=========================================================================================\n");

    for (int target_year = 1; target_year <= 4; target_year++) {
        int total_in_year = 0;
        int evaluated_in_year = 0;
        float total_cgpa_in_year = 0.0f;

        for (int i = 0; i < student_count; i++) {
            if (students[i].year == target_year) {
                total_in_year++;
                int valid_sems = 0;
                float student_cgpa = computeStudentCGPA(&students[i], &valid_sems);

                // Ignore students who do not have any available SGPA values
                if (valid_sems > 0) {
                    total_cgpa_in_year += student_cgpa;
                    evaluated_in_year++;
                }
            }
        }

        printf("Year %d:\n", target_year);
        printf("  Total Enrolled Students : %d\n", total_in_year);
        if (evaluated_in_year == 0) {
            if (total_in_year == 0) {
                printf("  Status                  : No students enrolled in this year.\n");
            } else {
                printf("  Status                  : Enrolled students have no SGPA entered yet.\n");
            }
            printf("  Average CGPA            : N/A\n");
        } else {
            float year_average = total_cgpa_in_year / (float)evaluated_in_year;
            printf("  Evaluated Students      : %d (with valid SGPA records)\n", evaluated_in_year);
            printf("  Average CGPA            : %.2f / 10.00\n", year_average);
        }
        printf("-----------------------------------------------------------------------------------------\n");
    }
}

/* ----------------------------------------------------------------------------
 * Core Function 7: sortBySAPID()
 * Sorts all stored students in ascending (increasing) order of their SAP ID.
 * - Algorithm: Bubble Sort (O(n^2) worst/average, O(n) best).
 * - Structure Swapping: Swaps entire 'struct Student' records.
 * ---------------------------------------------------------------------------- */
void sortBySAPID(struct Student students[], int student_count) {
    printf("\n--- [7] Sort by SAP ID ---\n");

    if (student_count == 0) {
        printf("[Notice] No student records available to sort.\n");
        return;
    }

    if (student_count == 1) {
        printf("[Notice] Only 1 student record present; it is already sorted.\n");
        displayAllStudents(students, student_count);
        return;
    }

    // Bubble Sort with early exit flag
    for (int i = 0; i < student_count - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < student_count - i - 1; j++) {
            if (students[j].sap_id > students[j + 1].sap_id) {
                struct Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
                swapped = 1;
            }
        }
        if (!swapped) {
            break;
        }
    }

    printf("\n[Success] Records successfully sorted by SAP ID in ascending order!\n");
    displayAllStudents(students, student_count);
}

/* ----------------------------------------------------------------------------
 * Core Function 8: sortByName()
 * Sorts all stored students alphabetically by Name using Bubble Sort & strcmp().
 * ---------------------------------------------------------------------------- */
void sortByName(struct Student students[], int student_count) {
    printf("\n--- [8] Sort by Name ---\n");

    if (student_count == 0) {
        printf("[Notice] No student records available to sort.\n");
        return;
    }

    if (student_count == 1) {
        printf("[Notice] Only 1 student record present; it is already sorted.\n");
        displayAllStudents(students, student_count);
        return;
    }

    // Bubble Sort alphabetically
    for (int i = 0; i < student_count - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < student_count - i - 1; j++) {
            if (strcmp(students[j].name, students[j + 1].name) > 0) {
                struct Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
                swapped = 1;
            }
        }
        if (!swapped) {
            break;
        }
    }

    printf("\n[Success] Records successfully sorted alphabetically by Name!\n");
    displayAllStudents(students, student_count);
}

/* ----------------------------------------------------------------------------
 * Core Function 9: academicAdvisor()
 * Rule-based decision support system providing personalized academic advice:
 * - Prompts for SAP ID and searches record.
 * - Computes CGPA from available SGPAs.
 * - Categorizes performance using exact if-else threshold rules:
 *     * CGPA >= 8.5 : Excellent Performance – Maintain your current study pattern.
 *     * CGPA >= 7.0 : Good Performance – Continue working consistently.
 *     * CGPA >= 5.5 : Needs Improvement – Focus more on subjects with lower SGPA.
 *     * CGPA < 5.5  : Requires Immediate Attention – Seek academic guidance and improve your study routine.
 * ---------------------------------------------------------------------------- */
void academicAdvisor(const struct Student students[], int student_count) {
    printf("\n--- [9] AI-Assisted Academic Advisor ---\n");

    if (student_count == 0) {
        printf("[Notice] No student records available. Please add students first.\n");
        return;
    }

    long long int target_sap;
    printf("Enter SAP ID of the student: ");
    if (scanf("%lld", &target_sap) != 1 || target_sap <= 0) {
        printf("[Error] Invalid SAP ID format.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int found_index = -1;
    for (int i = 0; i < student_count; i++) {
        if (students[i].sap_id == target_sap) {
            found_index = i;
            break;
        }
    }

    // Handle student not found
    if (found_index == -1) {
        printf("\n[Notice] Student Not Found: No record matches SAP ID %lld.\n", target_sap);
        return;
    }

    int valid_semesters = 0;
    float cgpa = computeStudentCGPA(&students[found_index], &valid_semesters);

    printf("\n==================== ACADEMIC ADVISOR REPORT ====================\n");
    printf("Student Name        : %s\n", students[found_index].name);
    printf("SAP ID              : %lld\n", students[found_index].sap_id);
    printf("Roll Number         : %d\n", students[found_index].roll_number);
    printf("Year of Study       : %d | Current Semester: %d\n",
           students[found_index].year, students[found_index].semester);
    printf("Evaluated Semesters : %d of %d\n", valid_semesters, TOTAL_SEMESTERS);

    // Check if any SGPA has been entered
    if (valid_semesters == 0) {
        printf("Calculated CGPA     : N/A\n");
        printf("Advisor Evaluation  : Insufficient academic data.\n");
        printf("Advisor Advice      : Please enter at least one semester SGPA to receive advice.\n");
        printf("=================================================================\n");
        return;
    }

    printf("Calculated CGPA     : %.2f / 10.00\n", cgpa);
    printf("-----------------------------------------------------------------\n");
    printf("Performance Status  : ");

    // Rule-based decision logic
    if (cgpa >= 8.5f) {
        printf("Excellent Performance\n");
        printf("Advisor Advice      : Maintain your current study pattern.\n");
    } else if (cgpa >= 7.0f) {
        printf("Good Performance\n");
        printf("Advisor Advice      : Continue working consistently.\n");
    } else if (cgpa >= 5.5f) {
        printf("Needs Improvement\n");
        printf("Advisor Advice      : Focus more on subjects with lower SGPA.\n");
    } else {
        printf("Requires Immediate Attention\n");
        printf("Advisor Advice      : Seek academic guidance and improve your study routine.\n");
    }
    printf("=================================================================\n");
}
