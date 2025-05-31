#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id, num_subjects;
    char name[100];
    float grades[10];
} Student;

Student* students[100];
int count = 0;

float average(float* grades, int n) {
    float sum = 0;
    for (int i = 0; i < n; i++) sum += grades[i];
    return n ? sum / n : 0;
}

void save_file() {
    char filename[100];
    printf("Enter filename: ");
    scanf("%s", filename);
    
    FILE* f = fopen(filename, "wb");
    if (!f) {
        printf("Error: '%s'!\n", filename);
        return;
    }
    fwrite(&count, sizeof(int), 1, f);
    for (int i = 0; i < count; i++)
        fwrite(students[i], sizeof(Student), 1, f);
    fclose(f);
    printf("Data saved successfully to '%s'! (%d students)\n", filename, count);
}

void save() {
    FILE* f = fopen("students.dat", "wb");
    if (!f) {
        printf("Error: \n");
        return;
    }
    fwrite(&count, sizeof(int), 1, f);
    for (int i = 0; i < count; i++)
        fwrite(students[i], sizeof(Student), 1, f);
    fclose(f);
}

void load() {
    FILE* f = fopen("students.dat", "rb");
    if (!f) return;
    fread(&count, sizeof(int), 1, f);
    for (int i = 0; i < count; i++) {
        students[i] = malloc(sizeof(Student));
        fread(students[i], sizeof(Student), 1, f);
    }
    fclose(f);
    if (count > 0) printf("Loaded %d students from file.\n", count);
}

void add_student() {
    if (count >= 100) {
        printf("Maximum students reached!\n");
        return;
    }
    Student* s = malloc(sizeof(Student));
    printf("Enter ID: "); scanf("%d", &s->id);
    
    for (int i = 0; i < count; i++) {
        if (students[i]->id == s->id) {
            printf("ID already exists!\n");
            free(s);
            return;
        }
    }
    
    getchar(); printf("Enter name: "); fgets(s->name, 100, stdin);
    s->name[strcspn(s->name, "\n")] = 0;
    printf("Number of subjects: "); scanf("%d", &s->num_subjects);
    for (int i = 0; i < s->num_subjects; i++) {
        printf("Grade %d: ", i + 1);
        scanf("%f", &s->grades[i]);
    }
    students[count++] = s;
    printf("Student added successfully!\n");
    save(); 
}

void display() {
    if (count == 0) { 
        printf("No students in the system.\n"); 
        return; 
    }
    printf("\n|STUDENT RECORDS|\n");
    for (int i = 0; i < count; i++) {
        Student* s = students[i];
        printf("\nStudent %d:\n", i + 1);
        printf("ID: %d\n", s->id);
        printf("Name: %s\n", s->name);
        printf("Grades: ");
        for (int j = 0; j < s->num_subjects; j++)
            printf("%.1f ", s->grades[j]);
        printf("\nAverage: %.2f\n", average(s->grades, s->num_subjects));
        printf("\n");
    }
    printf("Total students: %d\n", count);
}

void edit_student() {
    int id, found = 0;
    if (count == 0) {
        printf("No students to edit.\n");
        return;
    }
    printf("Enter ID to edit: "); scanf("%d", &id);
    for (int i = 0; i < count; i++) {
        if (students[i]->id == id) {
            Student* s = students[i];
            printf("Current name: %s\n", s->name);
            getchar(); printf("New name: "); fgets(s->name, 100, stdin);
            s->name[strcspn(s->name, "\n")] = 0;
            printf("New number of subjects: "); scanf("%d", &s->num_subjects);
            printf("Enter new grades:\n");
            for (int j = 0; j < s->num_subjects; j++) {
                printf("Grade %d: ", j + 1);
                scanf("%f", &s->grades[j]);
            }
            printf("Student updated successfully!\n"); 
            found = 1; 
            save(); 
            break;
        }
    }
    if (!found) printf("Student with ID %d not found.\n", id);
}

void delete_student() {
    int id;
    if (count == 0) {
        printf("No students to delete.\n");
        return;
    }
    printf("Enter ID to delete: "); scanf("%d", &id);
    for (int i = 0; i < count; i++) {
        if (students[i]->id == id) {
            printf("Student '%s' deleted successfully!\n", students[i]->name);
            free(students[i]);
            for (int j = i; j < count - 1; j++)
                students[j] = students[j + 1];
            count--; 
            save();
            return;
        }
    }
    printf("Student with ID %d not found.\n", id);
}

int main() {
    int ch; 
    printf("\nSTUDENT MANAGEMENT SYSTEM\n");
    load(); 
    
    while (1) { 
        printf("__________________\n");
        printf("1.Add Student\n");
        printf("__________________\n");
        printf("2.Display Student\n");
        printf("__________________\n");
        printf("3.Edit Student\n");
        printf("__________________\n");
        printf("4.Delete Student\n");
        printf("__________________\n");
        printf("5.Save \n");
        printf("__________________\n");
        printf("Enter your choice: ");
        printf("\n__________________\n");
        scanf("%d", &ch);
        
        if (ch == 1) add_student();
        else if (ch == 2) display();
        else if (ch == 3) edit_student();
        else if (ch == 4) delete_student();
        else if (ch == 5) save_file();
        else if (ch == 0) {
            printf("Thank You!\n");
            break;
        }
        else printf("Invalid:.\n");
    }
    return 0;
}