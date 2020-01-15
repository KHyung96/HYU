#include <stdio.h>
#include <stdlib.h>

typedef struct Tree
{
    int value;
    struct Tree *left;
    struct Tree *right;
}Tree;

Tree* insertNode(Tree *root,int key);
void printInorder(Tree *root);
void deleteTree(Tree *root);

int main(int argc,char* argv[])
{
    FILE *in = fopen(argv[1],"r");
    char cv;
    int key;
    
    Tree *root=NULL;
    
    while(!feof(in))
    {
        fscanf(in,"%c",&cv);
        if(cv == 'i')
        {
            fscanf(in,"%d",&key);
            root=insertNode(root,key);
        }
        else if(cv == 'p')
        {
            fscanf(in,"%c",&cv);
            if(cv == 'i')
			{
				if(root == NULL) printf("Print Error : Tree has no node.");
				else printInorder(root);
			}
            printf("\n");
        }
    }
    deleteTree(root);
    fclose(in);
    return 0;
}

Tree* insertNode(Tree *root,int key)
{
    if(root == NULL)
    {
        root=(Tree*)malloc(sizeof(Tree));
        root->value=key;
        root->left=root->right=NULL;
		printf("insert %d\n",key);
    }
    else if(root->value > key) root->left=insertNode(root->left,key);
    else if(root->value < key) root->right=insertNode(root->right,key);
	else if(root->value == key) printf("Insertion Error : There is already %d in tree.\n",key);
	return root;
}

void printInorder(Tree *root)
{
    if(root == NULL) return;
    printInorder(root->left);
    printf("%d ",root->value);
    printInorder(root->right);
}

void deleteTree(Tree *root)
{
    if(root == NULL) return;
    deleteTree(root->left);
    deleteTree(root->right);
    free(root);
}

