#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"

#define TREE_CMD_INDENT_SIZE 4
#define NO_ARG ""
#define PARENT_DIR ".."

// functie care creeaza directorul root
// este apelata automat la rularea progamului
FileTree createFileTree(char* rootFolderName) {
    FileTree tree;

    tree.root = malloc(sizeof(TreeNode));
    tree.root->name = rootFolderName;
    tree.root->parent = NULL;
    tree.root->type = FOLDER_NODE;

    tree.root->content = malloc(sizeof(FolderContent));
    ((FolderContent *)tree.root->content)->children = ll_create();

    return tree;
}

// functie folosita pentru a aloca si crea un fisier
TreeNode* createFile(char* name, char* text)
{
    TreeNode* new_file = malloc(sizeof(TreeNode));

    new_file->type = FILE_NODE;
    new_file->name = name;
    new_file->parent = NULL;

    new_file->content = malloc(sizeof(FileContent));

    ((FileContent*)new_file->content)->text = text;

    return new_file;
}

// similara cu createFileTree, dar pentru orice director
// intoarce un pointer TreeNode, nu FileTree ca createFileTree
TreeNode* createDir(char* name)
{
    TreeNode* new_folder = malloc(sizeof(TreeNode));

    new_folder->type = FOLDER_NODE;
    new_folder->name = name;
    new_folder->parent = NULL;

    new_folder->content = malloc(sizeof(FolderContent));

    ((FolderContent*)new_folder->content)->children = ll_create();

    return new_folder;
}

// functie care se apeleaza inainte de a se opri executia programului
// sterge toate fisierele si directoarele din root si elibereaza memoria
void freeTree(FileTree fileTree) {
    ListNode *current;
    current = ((FolderContent *)fileTree.root->content)->children->head;

    while (current) {
        rmrec(fileTree.root, current->info->name);
        current = ((FolderContent *)fileTree.root->content)->children->head;
    }

    free(((FolderContent *)fileTree.root->content)->children);
    free(fileTree.root->content);
    free(fileTree.root->name);
    free(fileTree.root);
}

// afla calea catre un element dat
TreeNode *get_path(TreeNode *currentNode, char *path) {
    char *buffer = malloc(strlen(path) + 1);
    strcpy(buffer, path);

    char *token = strtok(buffer, "/");

    TreeNode *current_path = currentNode;
    ListNode *current;

    while (token) {
        if (strcmp(token, PARENT_DIR) == 0) {
            current_path = current_path->parent;
        } else {
            current = ((FolderContent*)current_path->content)->children->head;
            while (current) {
                if (strcmp(token, current->info->name) == 0)
                    break;
                current = current->next;
            }
            if (current == NULL) {
                free(buffer);
                return NULL;
            }
            current_path = current->info;
        }
        if (current_path == NULL) {
            free(buffer);
            return current_path;
        }

        token = strtok(NULL, "/");
    }

    free(buffer);
    return current_path;
}

// ls
// functie care ia ca parametru un fisier sau directory
// daca arg exista, acesta va afisa continutul fisierului <arg>:
// <arg>: <arg_content>
// daca <arg> nu exista, functia va afisa eroare de mai jos
void ls(TreeNode* currentNode, char* arg) {
    TreeNode *path = get_path(currentNode, arg);
    if (!path) {
        printf("ls: cannot access '%s': No such file or directory\n", arg);
        return;
    }

    // se verifica daca nodul este fisier si se afiseaza continutul
    if (path->type == FILE_NODE) {
        printf("%s: %s\n", path->name, ((FileContent*)path->content)->text);
        return;
    }

    // in cazul in care este dat un directory ca input
    // se afiseaza toate elementele din lista de copii a directorului
    ll_print_string(((FolderContent*)path->content)->children);
}

// functie care afiseaza calea unui fisier sau directory, relativa la
void pwd(TreeNode* treeNode) {
    char *path_name = malloc(strlen(treeNode->name) + 1);
    strcpy(path_name, treeNode->name);

    // string temporar folosit pentru a concatena in path_name
    // numele directoarelor
    char *auxiliary_string;

    TreeNode* current = treeNode->parent;

    while (current != NULL) {
        int string_size = strlen(path_name) + strlen(current->name) +2;
        auxiliary_string = malloc(string_size);
        strcpy(auxiliary_string, current->name);
        auxiliary_string[strlen(current->name)] = '/';
        auxiliary_string[strlen(current->name) + 1] = '\0';

        strcat(auxiliary_string, path_name);

        free(path_name);
        path_name = malloc(strlen(auxiliary_string) + 1);
        strcpy(path_name, auxiliary_string);

        current = current->parent;
        free(auxiliary_string);
    }

    printf("%s\n", path_name);
    free(path_name);
}

// functie care primeste ca parametru un path
// si intoarce directorul destinatie
// daca nu se poate ajunge la destinatia data sau nu exista,
// va afisa un mesaj de eroare
TreeNode* cd(TreeNode* currentNode, char* path) {
    TreeNode *current_path = get_path(currentNode, path);

    if (current_path == NULL) {
        printf("cd: no such file or directory: %s\n", path);
        return currentNode;
    }

    return current_path;
}

void print_tree(TreeNode *currentNode, int tabs, int *contents) {
    ListNode *current = ((FolderContent*)currentNode->content)->children->head;

    while (current != NULL) {
        int i;
        for (i = 0; i < tabs; i++)
            printf("\t");
        printf("%s\n", current->info->name);

        if (current->info->type == FILE_NODE) {
            contents[1]++;
        } else {
            contents[0]++;
            print_tree(current->info, tabs + 1, contents);
        }
        current = current->next;
    }
}

// afiseaza ierarhia de fisiere si directoare pornind de la argumentul dat
void tree(TreeNode* currentNode, char* arg) {
    int contents[2] = {0};
    // in cazul in care nu este dat un argument, se afiseaza toata ierarhia
    // de fisiere, pornind de la root
    if (arg == NULL) {
        print_tree(currentNode, 0, contents);
    } else {
        TreeNode *path = get_path(currentNode, arg);
        if (path == NULL || path->type  == FILE_NODE) {
            printf("%s [error opening dir]\n\n0 directories, 0 files\n", arg);
            return;
        }
        print_tree(path, 0, contents);
    }
    printf("%d directories, %d files\n", contents[0], contents[1]);
}

// comanda care creeaza un director in directorul curent
// daca un director cu acelasi nume dat la input exista, se va afisa o eroare
void mkdir(TreeNode* currentNode, char* folderName) {
    ListNode* current = ((FolderContent*)currentNode->content)->children->head;

    // se verifica daca exista deja un director cu acelasi nume
    while (current != NULL) {
        if (strcmp(current->info->name, folderName) == 0) {
            printf("mkdir: cannot create directory '%s': File exists\n",
             folderName);
            return;
        }
        current = current->next;
    }

    TreeNode *new_dir = createDir(folderName);
    new_dir->parent = currentNode;

    // se adauga directorul nou in cel in care ne aflam(dat ca parametru)
    ll_add_nth_node
    (((FolderContent *)currentNode->content)->children, 0, new_dir);
}

// comanda care sterge un director sau fisier, indiferent
// daca aceste este gol sau nu
void rmrec(TreeNode* currentNode, char* resourceName) {
    TreeNode *node = get_path(currentNode, resourceName);

    // se verifica daca elementul dat ca input se afla la calea data
    if (node == NULL) {
        printf("rmrec: failed to remove '%s': No such file or directory\n",
         resourceName);
        return;
    }

    if (node->type == FILE_NODE) {
        rm(node->parent, node->name);
        return;
    }

    // daca directorul dat este gol, este sters imediat
    if (node->type == FOLDER_NODE &&
     ((FolderContent *)node->content)->children->head == NULL) {
        rmdir(node->parent, node->name);
        return;
    }

    ListNode *current = ((FolderContent *)node->content)->children->head;
    ListNode *outer;

    // continutul directorului este sters recursiv, functia reapelandu-se
    // insasi pana nu mai ramane nimic
    while (current) {
        rmrec(node, current->info->name);
        current = ((FolderContent *)node->content)->children->head;
    }

    rmdir(node->parent, node->name);
}

// comanda care sterge un fisier dat ca parametru
void rm(TreeNode* currentNode, char* fileName) {
    ListNode *current = ((FolderContent *)currentNode->content)->children->head;
    int counter = 0;

    while (current) {
        if (strcmp(fileName, current->info->name) == 0)
            break;
        current = current->next;
        counter++;
    }

    // se verifica daca exista fisierul dat in directorul curent
    if (current == NULL) {
        printf("rm: failed to remove '%s': No such file or directory\n",
         fileName);
        return;
    }

    // daca inputul este un director, se afiseaza o eroare
    if (current->info->type == FOLDER_NODE) {
        printf("rm: cannot remove '%s': Is a directory\n", fileName);
        return;
    }

    // se sterge nodul care contine fisierul si
    // se elibereaza memoria din locul ramas
    ListNode *removed = ll_remove_nth_node
    (((FolderContent *)currentNode->content)->children, counter);

    free(((FileContent *)removed->info->content)->text);
    free(removed->info->content);
    free(removed->info->name);
    free(removed->info);
    free(removed);
}

// comanda care sterge un director, dar numai daca acesta este gol
void rmdir(TreeNode* currentNode, char* folderName) {
    ListNode *current =
    ((FolderContent *)currentNode->content)->children->head;
    int counter = 0;

    // se afla pozitia din lista de copii ai directorului in care
    // se afla directorul pe care vrem sa il stergem
    while (current) {
        if (strcmp(folderName, current->info->name) == 0)
            break;
        current = current->next;
        counter++;
    }

    // daca nu exista, se afiseaza eroare de mai jos
    if (current == NULL) {
        printf("rmdir: failed to remove '%s': No such file or directory\n",
         folderName);
        return;
    }

    // se verifica tipul nodului dat ca input
    if (current->info->type == FILE_NODE) {
        printf("rmdir: failed to remove '%s': Not a directory\n",
         folderName);
        return;
    }

    List *list = ((FolderContent *)current->info->content)->children;

    // daca directorul nu este gol, comanda nu va functiona
    if (list->head != NULL) {
        printf
        ("rmdir: failed to remove '%s': Directory not empty\n", folderName);
        return;
    }

    // se sterge directorul si i se elibereaza memoria
    free(list);

    ListNode *removed = ll_remove_nth_node
                    (((FolderContent*)currentNode->content)->children, counter);

    free(removed->info->content);
    free(removed->info->name);
    free(removed->info);
    free(removed);
}

// comanda care creeaza un fisier si respectiv scrie inauntrul acestuia
void touch(TreeNode* currentNode, char* fileName, char* fileContent) {
    ListNode* current = ((FolderContent*)currentNode->content)->children->head;

    // verificam daca exista deja fisierul
    while (current != NULL) {
        if (strcmp(current->info->name, fileName) == 0)
            return;
        current = current->next;
    }

    TreeNode* file = createFile(fileName, fileContent);
    file->parent = currentNode;
    ll_add_nth_node
        (((FolderContent *)currentNode->content)->children, 0, file);
}

// comanda care copiaza fisierul dat ca argument in directorul dat
// daca exista un fisier deja existent cu numele "source",
// continutul acestuia va fi inlocuit
// daca path ul dat nu poate fi accesat sau daca path-ul este un fisier,
// se va afisa un mesaj de eroare
void cp(TreeNode* currentNode, char* source, char* destination) {
    TreeNode *source_node = get_path(currentNode, source);
    TreeNode *dest_node = get_path(currentNode, destination);

    if (dest_node == NULL) {
        printf("cp: failed to access '%s': Not a directory\n", destination);
        return;
    }

    // fc. ia ca parametru numai noduri tip fisier
    if (source_node->type == FOLDER_NODE) {
        printf("cp: -r not specified; omitting directory '%s'\n", source);
    }

    // se verifica daca fisierul destination exista deja
    // daca exista, i se elibereaza memoria si se aloca iarasi
    // pt dimensiunea noului content
    // apoi se adauga in continutul sau, continutul din source
    if (source_node->type == FILE_NODE && dest_node->type == FILE_NODE) {
        free(((FileContent *)dest_node->content)->text);

        ((FileContent *)dest_node->content)->text =
        malloc(strlen(((FileContent *)source_node->content)->text)+1);

        strcpy(((FileContent *)dest_node->content)->text,
            ((FileContent *)source_node->content)->text);
        return;
    }

    char *name = malloc(strlen(source_node->name) + 1);
    strcpy(name, source_node->name);

    char *text =
        malloc(strlen(((FileContent *)source_node->content)->text) + 1);
    strcpy(text, ((FileContent *)source_node->content)->text);

    touch(dest_node, name, text);
}

// comanda care muta un fisier sau un director catre destinatia data
// similara cu cp, dar ia ca parametru si un director
void mv(TreeNode* currentNode, char* source, char* destination) {
    TreeNode *source_node = get_path(currentNode, source);
    ListNode *current =
    ((FolderContent *)source_node->parent->content)->children->head;

    int counter = 0;

    while (strcmp(current->info->name, source_node->name) != 0) {
        current = current->next;
        counter++;
    }

    TreeNode *dest_node = get_path(currentNode, destination);

    if (dest_node == NULL) {
        printf("mv: failed to access '%s': Not a directory\n", destination);
        return;
    }

    // se verifica daca exista un fisier deja existent cu acelasi nume
    if (source_node->type == FILE_NODE && dest_node->type == FILE_NODE) {
        free(((FileContent *)dest_node->content)->text);

        // se face overwrite in cazul in care exista deja
        // un fisier cu acelasi nume
        ((FileContent *)dest_node->content)->text =
            ((FileContent *)source_node->content)->text;

        // este eliberata memoria in locul de unde a fost mutat nodul
        ListNode *removed = ll_remove_nth_node
        (((FolderContent *)source_node->parent->content)->children, counter);

        free(removed->info->content);
        free(removed->info->name);
        free(removed->info);
        free(removed);

        return;
    }
    ListNode *removed = ll_remove_nth_node
        (((FolderContent *)source_node->parent->content)->children, counter);

    removed->info->parent = dest_node;

    ll_add_nth_node
        (((FolderContent *)dest_node->content)->children, 0, removed->info);

    free(removed);
}
