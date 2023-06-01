#include<stdio.h>
#include<stdlib.h>
#define MAX_LINE_LENGTH 1000

struct node{
    int val;
    struct node* left;
    struct node* right;
};

struct heap{
    int *arr;
    int heap_len;
    int arr_len;
    int size;
};

struct code{
    char c;
    int freq;
    char * code;
};

struct node* allocate_node(int val){
    struct node* n=(struct node*)malloc(sizeof(struct node));
    n->val=val;
    n->left=NULL;
    n->right=NULL;
    return n;
}

struct heap* allocate_heap(int size){
    struct heap* H=(struct heap*)malloc(sizeof(struct heap));
    H->arr=(int *)malloc(size*sizeof(int));
    H->arr_len=size-1;
    H->heap_len=size-1;
    H->size=size-1;
    return H;
}

void heapify_at(int idx, int* arr, int size);
void build_minHeap(struct heap* H);
int extract_min(struct heap* H);
void insert_at(int* arr, int idx);
void insert_minHeap(struct heap* H, int val);
void min_heapify(struct heap* H, int idx);
struct node* find(int val, struct node* arr2[], int len);
int check(int val, struct node* arr2[], int len);
void assign_code(int freq[], struct node* root, char code[][100], int top, int len);
struct code* allocate_code(char c, int freq);
int match_code(char code[][100], char str[], int len, int i);
int member(char c, char arr[], int idx);
int get(char c, char arr[], int len);

void heapify_at(int idx, int* arr, int size){
    int left=2*idx;
    int right=2*idx+1;
    int smallest=idx;
    if(left<=size){
        if(arr[smallest]>arr[left] && left<=size){
            smallest=left;
        }
    }
    if(right<=size){
        if(arr[smallest]>arr[right] && right<=size){
            smallest=right;
        }
    }
    if(smallest!=idx){
        int tmp=arr[idx];
        arr[idx]=arr[smallest];
        arr[smallest]=tmp;
        heapify_at(smallest, arr, size);
    }
    return;
}

void assign_code(int freq[], struct node* root, char code[][100], int top, int len){
    static char arr[100];
    if(root->left!=NULL){
        arr[top]='0';
        assign_code(freq, root->left, code, top+1, len);
    }
    if(root->right!=NULL){
        arr[top]='1';
        assign_code(freq, root->right, code, top+1, len);
    }
    if(root->left==NULL && root->right==NULL){
        for(int i=0; i<len; i++){
            if(root->val==freq[i]){
                int j;
                for(j=0; j<top; j++){
                    code[i][j]=arr[j];
                }
                code[i][j]='\0';
            }
        }
    }
    return;
}

void build_minHeap(struct heap* H){
    int i;
    int n=H->arr_len;
    for(i=n/2; i>=1; i--){
        min_heapify(H, i);
    }
    return;
}

int member(char c, char arr[], int idx){
    for(int i=0; i<idx; i++){
        if(arr[i]==c){
            return 1;
        }
    }
    return 0;
}

int extract_min(struct heap* H){
    int tmp=H->arr[1];
    H->arr[1]=H->arr[H->heap_len];
    H->arr[H->heap_len]=tmp;
    H->heap_len-=1;
    min_heapify(H, 1);
    return tmp;
}

void insert_at(int* arr, int idx){
    if(idx==1){
        return;
    }
    int parent=idx/2;
    if(arr[parent]>arr[idx]){
        int tmp=arr[parent];
        arr[parent]=arr[idx];
        arr[idx]=tmp;
        insert_at(arr, parent);
    }
    return;
}

void insert_minHeap(struct heap* H, int val){
    int n=H->heap_len;
    H->arr[n+1]=val;
    insert_at(H->arr, n+1);
    H->heap_len+=1;
    return;
}

struct node* find(int val, struct node* arr2[], int len){
    for(int i=0; i<2*len-1; i++){
        if(arr2[i]->val==val){
            return arr2[i];
        }
    }
}

void inorder(struct node* root){
    if(root==NULL){
        return;
    }
    inorder(root->left);
    printf("%d\t", root->val);
    inorder(root->right);
    return;
}

void min_heapify(struct heap* H, int idx){
    heapify_at(idx, H->arr, H->heap_len);
    return;
}

int get(char c, char arr[], int len){
    for(int i=0; i<len; i++){
        if(arr[i]==c){
            return i;
        }
    }
}

int check(int val, struct node* arr2[], int len){
    for(int i=0; i<2*len-1; i++){
        if(arr2[i]->val==val){
            return 1;
        }
    }
    return 0;
}

int match_code(char code[][100], char str[], int len, int i){
    for(int j=0; j<len; j++){
        int flag=1;
        int count=0;
        int k=0;
        while(code[j][k]!='\0'){
            count+=1;
            k+=1;
        }
        if(count==i){
            for(int l=0; l<i; l++){
                if(code[j][l]!=str[l]){
                    flag=0;
                    break;
                }
            }
            if(flag==1){
                return j;
        }
        else{
            continue;
        }
        }
    }
    return -1;
}

struct code* allocate_code(char c, int freq){
    struct code* C=(struct code*)malloc(sizeof(struct code));
    C->c=c;
    C->freq=freq;
    C->code=(char *)malloc(100*sizeof(char));
}

int main(){
    FILE* file;
    file=fopen("in.txt", "r");
    char arr[10000];
    int freq[10000];
    int idx=0;
    char c;
    while((c=fgetc(file))!=EOF){
        if(member(c, arr, idx)==0){
            arr[idx]=c;
            idx+=1;
        }
    }
    int len=idx;
    int count;
    fclose(file);
    for(int i=0; i<len; i++){
        file=fopen("in.txt", "r");
        count=0;
        while((c=fgetc(file))!=EOF){
            if(c==arr[i]){
                count+=1;
            }
        }
        freq[i]=count;
        fclose(file);
    }
    struct heap* H=allocate_heap(10000);
    H->heap_len=len;
    H->arr_len=len;
    H->size=len;
    for(int i=0; i<len; i++){
        H->arr[i+1]=freq[i];
    }
    struct node* test=allocate_node(0);
    build_minHeap(H);
    struct node* arr2[len];
    for(int i=0; i<2*len-1; i++){
        arr2[i]=test;
    }
    int i=0;
    struct node* m1;
    struct node* m2;
    struct node* m3;
    while(1){
        int min1=extract_min(H);
        int min2=extract_min(H);
        if(check(min1, arr2, len)==0){
            m1=allocate_node(min1);
            arr2[i++]=m1;
        }
        else{
            m1=find(min1, arr2, len);
        }
        if(check(min2, arr2, len)==0){
            m2=allocate_node(min2);
            arr2[i++]=m2;
        }
        else{
            m2=find(min2, arr2, len);
        }
        if(check(min1+min2, arr2, len)==0){
            m3=allocate_node(min1+min2);
            arr2[i++]=m3;
        }
        else{
            m3=find(min1+min2, arr2, len);
        }
        m3->left=m1;
        m3->right=m2;
        insert_minHeap(H, min1+min2);
        if(H->heap_len==1){
            break;
        }
    }
    struct node* root=m3;
    char code[10000][100];
    assign_code(freq, root, code, 0, len);
    FILE* new_file;
    char name1[100];
    char name2[100];
    int mode;
    printf("Enter the name of the input file: ");
    scanf("%s", &name1);
    printf("Enter the name of the output file: ");
    scanf("%s", &name2);
    printf("Enter Mode: ");
    scanf("%d", &mode);
    file=fopen(name1, "r");
    new_file=fopen(name2, "w");
    //Compressing
    if(mode==0){
        printf("Generating %s", name2);
        for(int i=0; i<len; i++){
            fprintf(new_file, "%c->%s", arr[i], code[i]);
            fprintf(new_file, "\n");
        }
        while((c=fgetc(file))!=EOF){
            idx=get(c, arr, len);
            fprintf(new_file,"%s", code[idx]);
        }
        fclose(file);
        fclose(new_file);
    }
    //Decompressing
    else if(mode==1){
        printf("Generating %s", name2);
        char str[10000];
        char s[10000];
        int i=0;
        int count=0;
        while(fgets(s, MAX_LINE_LENGTH, file)){
            if(s[1]=='-'){
                count+=1;
            }
            if(count==len){
                break;
            }
        }
        while((c=fgetc(file))!=EOF){
            str[i]=c;
            i+=1;
            if(match_code(code, str, len, i)!=-1){
                fprintf(new_file, "%c", arr[match_code(code, str, len, i)]);
                i=0;
            }
        }
    }
}