#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    int id;
    char name[20];
    float salary;
} Employee;

int main() {
    /* O_CREAT: create if missing, O_RDWR: read+write, 0600: owner read/write only */
    int fd = open("database.dat", O_CREAT | O_RDWR, 0600);
    if (fd < 0) return 1;

    Employee emp1 = {1, "Alice", 50000.0};
    Employee emp2 = {2, "Bob", 60000.0};
    write(fd, &emp1, sizeof(Employee));
    write(fd, &emp2, sizeof(Employee));
    printf("Initial records written.\n");

    /* Jump directly to record 2's byte offset instead of rewriting the file */
    lseek(fd, sizeof(Employee) * 1, SEEK_SET);
    Employee emp2_update = {2, "Bob", 65000.0};
    write(fd, &emp2_update, sizeof(Employee));
    printf("Record 2 updated.\n");

    /* Seek back to the start and read record 1 */
    lseek(fd, 0, SEEK_SET);
    Employee read_emp;
    read(fd, &read_emp, sizeof(Employee));
    printf("Retrieved Record: ID=%d, Name=%s, Salary=%.2f\n", read_emp.id, read_emp.name, read_emp.salary);

    close(fd);
    return 0;
}
