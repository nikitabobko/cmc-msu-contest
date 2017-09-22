#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>

typedef struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
} Node;

// returns 0 if all is ok; otherwise returns -1
int
read_node(int fd, Node *node)
{
    ssize_t really_read = 0;
    do {
        if (lseek(fd, -really_read, SEEK_CUR) == -1 || 
            (really_read = read(fd, node, sizeof(*node))) == -1 || really_read == 0) {
            return -1;
        }
    } while(really_read != sizeof(*node));
    return 0;
}

// returns 0 if all is ok; otherwise returns -1
int 
print_tree(int fd, int index) 
{
    Node node;
    if (lseek(fd, index*sizeof(node), SEEK_SET) == -1 || read_node(fd, &node) == -1) {
        return -1;
    }
    if (node.left_idx != 0) {
        print_tree(fd, node.left_idx);
    }
    printf("%d\n", node.key);
    if (node.right_idx != 0) {
        print_tree(fd, node.right_idx);
    }
    return 0;
}

int 
main(int argc, char const *argv[])
{
    if (argc != 2) {
        return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        return 1;
    }
    print_tree(fd, 0);
    return 0;
}
