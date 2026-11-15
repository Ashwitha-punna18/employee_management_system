#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "employees.dat"

struct Employee
{
    int id;
    char name[50];
    char department[30];
    float salary;
};

void addEmployee();
void displayEmployees();
void searchEmployee();
void updateEmployee();
void deleteEmployee();

int main()
{
    int choice;

    while (1)
    {
        printf("\n===== EMPLOYEE MANAGEMENT SYSTEM =====\n");
        printf("1. Add Employee\n");
        printf("2. Display Employees\n");
        printf("3. Search Employee\n");
        printf("4. Update Employee\n");
        printf("5. Delete Employee\n");
        printf("6. Exit\n");

        printf("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input!\n");
            return 1;
        }

        switch (choice)
        {
            case 1:
                addEmployee();
                break;

            case 2:
                displayEmployees();
                break;

            case 3:
                searchEmployee();
                break;

            case 4:
                updateEmployee();
                break;

            case 5:
                deleteEmployee();
                break;

            case 6:
                printf("Thank you!\n");
                return 0;

            default:
                printf("Invalid choice!\n");
        }
    }
}

void addEmployee()
{
    struct Employee e, temp;

    FILE *fp = fopen(FILE_NAME, "ab+");

    if (fp == NULL)
    {
        printf("File opening error!\n");
        return;
    }

    printf("Enter Employee ID: ");
    scanf("%d", &e.id);

    rewind(fp);

    while (fread(&temp, sizeof(temp), 1, fp) == 1)
    {
        if (temp.id == e.id)
        {
            printf("Employee ID already exists!\n");
            fclose(fp);
            return;
        }
    }

    printf("Enter Name: ");
    scanf(" %49[^\n]", e.name);

    printf("Enter Department: ");
    scanf(" %29[^\n]", e.department);

    printf("Enter Salary: ");
    scanf("%f", &e.salary);

    fseek(fp, 0, SEEK_END);
    fwrite(&e, sizeof(e), 1, fp);

    fclose(fp);

    printf("Employee added successfully!\n");
}

void displayEmployees()
{
    struct Employee e;

    FILE *fp = fopen(FILE_NAME, "rb");

    if (fp == NULL)
    {
        printf("No employee records found!\n");
        return;
    }

    printf("\nID\tName\t\tDepartment\tSalary\n");

    while (fread(&e, sizeof(e), 1, fp) == 1)
    {
        printf("%d\t%-15s\t%-15s\t%.2f\n",
               e.id, e.name, e.department, e.salary);
    }

    fclose(fp);
}

void searchEmployee()
{
    struct Employee e;
    int id, found = 0;

    FILE *fp = fopen(FILE_NAME, "rb");

    if (fp == NULL)
    {
        printf("No records found!\n");
        return;
    }

    printf("Enter Employee ID to search: ");
    scanf("%d", &id);

    while (fread(&e, sizeof(e), 1, fp) == 1)
    {
        if (e.id == id)
        {
            printf("\nEmployee Found!\n");
            printf("ID: %d\n", e.id);
            printf("Name: %s\n", e.name);
            printf("Department: %s\n", e.department);
            printf("Salary: %.2f\n", e.salary);

            found = 1;
            break;
        }
    }

    if (!found)
        printf("Employee not found!\n");

    fclose(fp);
}

void updateEmployee()
{
    struct Employee e;
    int id, found = 0;

    FILE *fp = fopen(FILE_NAME, "rb+");

    if (fp == NULL)
    {
        printf("No records found!\n");
        return;
    }

    printf("Enter Employee ID to update: ");
    scanf("%d", &id);

    while (fread(&e, sizeof(e), 1, fp) == 1)
    {
        if (e.id == id)
        {
            printf("Enter New Name: ");
            scanf(" %49[^\n]", e.name);

            printf("Enter New Department: ");
            scanf(" %29[^\n]", e.department);

            printf("Enter New Salary: ");
            scanf("%f", &e.salary);

            fseek(fp, -(long)sizeof(e), SEEK_CUR);

            fwrite(&e, sizeof(e), 1, fp);

            found = 1;
            printf("Employee updated successfully!\n");
            break;
        }
    }

    if (!found)
        printf("Employee not found!\n");

    fclose(fp);
}

void deleteEmployee()
{
    struct Employee e;
    int id, found = 0;

    FILE *fp = fopen(FILE_NAME, "rb");

    if (fp == NULL)
    {
        printf("No records found!\n");
        return;
    }

    FILE *temp = fopen("temp.dat", "wb");

    if (temp == NULL)
    {
        fclose(fp);
        printf("Temporary file error!\n");
        return;
    }

    printf("Enter Employee ID to delete: ");
    scanf("%d", &id);

    while (fread(&e, sizeof(e), 1, fp) == 1)
    {
        if (e.id == id)
        {
            found = 1;
            continue;
        }

        fwrite(&e, sizeof(e), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    if (found)
    {
        if (remove(FILE_NAME) == 0 &&
            rename("temp.dat", FILE_NAME) == 0)
            printf("Employee deleted successfully!\n");
        else
            printf("File replacement error!\n");
    }
    else
    {
        remove("temp.dat");
        printf("Employee not found!\n");
    }
}
