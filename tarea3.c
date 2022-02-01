#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>

typedef struct inode{
    char* name;
    int type;
    struct inode* next;
    struct inode* child;
    char* path;
}inode_n;

typedef struct lista{
    inode_n* root;
    inode_n* pointer;
}lista_n;

lista_n MoverA(lista_n pfs, char* ruta){
    inode_n flecha = *pfs.root;
    int var = true;
    while(var){
            if(flecha.next == NULL || ruta == NULL){
                perror("no existe la ruta");
            }
            if(flecha.path == ruta && ruta != NULL){
                var = false;
            }
            flecha = *flecha.next;
    }
    pfs.pointer = &flecha;
    
    
    return pfs;
    }

lista_n CrearCarpeta(lista_n pfs, char* name){
    if(pfs.pointer->type == 0){
        perror("error, la ruta actual es un archivo, muevete a una carpeta");
    }
    char* path = strcat(pfs.pointer->path, strcat("/", name));
    int result = mkdir(path, 0777);
    if (result == -1){
        perror("error en crear carpeta");
    }
    if (pfs.pointer->child == NULL){
        inode_n child = {
            .name = name,
            .type = 1,
            .path = path,
            .next = NULL,
            .child = NULL,
        };
    pfs.pointer->child = &child;    //lista
    pfs.pointer->next = &child;     
    lista_n pfsnew = MoverA(pfs, path);
    
    pfsnew.root = malloc(sizeof(inode_n));
    pfsnew.pointer = malloc(sizeof(inode_n));
    return pfsnew;
    };
}

lista_n CrearCausa(lista_n pfs, char* name){
    if(pfs.pointer->type == 0){
        perror("error, la ruta actual es un archivo, muevete a una carpeta");
    }
    char* path = strcat(pfs.pointer->path, strcat("/", name));
    int result = creat(path, 0777);
    if (result == -1){
        perror("error en crear archivo");
    }
    if (pfs.pointer->child == NULL){
        inode_n child = {
            .name = name,
            .type = 0,
            .path = path,
            .next = NULL,
            .child = NULL,
        };
        pfs.pointer->next = &child; 
        pfs.pointer->child = &child;    //lista
        lista_n pfsnew = MoverA(pfs, path);
        
        pfs.pointer->next = malloc(sizeof(inode_n));
        pfs.pointer->child = malloc(sizeof(inode_n));
        
        return pfsnew;
    }
}


lista_n Borrar(lista_n pfs){
    inode_n flecha = *pfs.root;
    bool var = true;
    while(var){
        if(flecha.next == pfs.pointer){
            
            if (flecha.next->type == 0){
                if(pfs.pointer->next != NULL) {
                    inode_n ayuda = *pfs.pointer->next;
                    pfs.pointer->next = ayuda.next;
                }
                int result1 = unlink(pfs.pointer->path);
                var = false;
                if (result1 == -1){
                perror("error en eliminar archivo");
            }
            }
            if (flecha.next->type == 1){
                if(pfs.pointer->next != NULL && pfs.pointer->child == NULL) {
                    inode_n ayuda = *pfs.pointer->next;
                    pfs.pointer->next = ayuda.next;
                }
                int result2 = rmdir(pfs.pointer->path);
                var = false;
                if (result2 == -1){
                    perror("error en eliminar carpeta, confirma que esta este vacia");
            }    
            }   
        }
        flecha = *flecha.next;
    }
    return pfs;
}   


lista_n MoverCausa(lista_n pfs, char* ruta){
    inode_n flecha = *pfs.root;
    char* name = pfs.pointer->name;
    bool var = true;
    if(pfs.pointer->type == 0){   
        while(var){
            if(flecha.next == NULL || ruta == NULL){
                perror("no existe la ruta");
            }
            if(flecha.path == ruta && ruta != NULL){
                char* pathNew = strcat(pfs.pointer->path, strcat("/", name));
                pfs.pointer->path == pathNew;
                int result = link(pfs.pointer->path, ruta);
                return pfs;
                var = false;
            }
            flecha = *pfs.pointer->next;
            
            
        }
    }else{
        perror("lamamadelamamadelamamadelamamadelamama");
    }

}

void ModificarCausa(lista_n pfs){       
}

/*void Archivar(lista_n pfs, char* rutaArchivar){
    node_n* flecha = pfs.root;
    if (pfs.pointer->type == 0){   
        while(true){
            if(flecha.path == rutaArchivar){
                
            }
            flecha = flecha.next;
        }
}
*/
lista_n lista_inicio(){ 
    char cwd[1024];
	getcwd(cwd,sizeof(cwd));
	inode_n root = {
		.name = "root",
		.type = 0,
		.path = &cwd[0],
		.next = NULL,
		.child = NULL,
	};
	lista_n pfs = {
		.root = &root,
		.pointer = &root,
	};
    pfs.root = malloc(sizeof(inode_n));
    pfs.pointer = malloc(sizeof(inode_n));
	return pfs;
}

int main(int argc, char *argv[ ]){
    int opcion; 
    lista_n pfs = lista_inicio();
    printf("Bienvenido al Sistema PFS\n");
    printf("%s\n%s\n",pfs.root->name,pfs.root->path);  
    printf("%s\n",pfs.pointer->path); 
    while(true){ 
        printf("actualmente estas en: %s\n",pfs.pointer->path); 
        printf("que quiere hacer? \n");
        printf("1. Crear Carpeta\n");
        printf("2. Borrar Carpeta\n");
        printf("3. Crear Causa\n");
        printf("4. Mover Causa\n");
        printf("5. Borrar Causa\n");
        printf("6. Archivar Causa\n");
        printf("7. Modificar Causa\n");
        printf("8. Moverse a\n");
        printf("9. Salir\n");
        scanf("%d",&opcion);
        switch(opcion){
            case 1:
                printf("Ingresar Nombre de carpeta\n");
                char* nombre;
                scanf("%c",nombre);
                CrearCarpeta(pfs, nombre);
                char* path = strcat(pfs.pointer->path, strcat("/", nombre));
                pfs = MoverA(pfs, path);
            case 2:
                printf("Borrar Carpeta");
                pfs = Borrar(pfs);
            case 3:
                printf("Ingrese el nombre de la causa\n");
                scanf("%c",nombre);
                CrearCausa(pfs, nombre);
                path = strcat(pfs.pointer->path, strcat("/", nombre));
                pfs = MoverA(pfs, path);
            case 4:
                printf("Ingrese la ruta a la que se movera la causa\n");
                char* ruta;
                scanf("%c",ruta);
                pfs = MoverCausa(pfs, ruta);
            case 5:
                printf("Borrar Carpeta");
                pfs = Borrar(pfs);
            case 6:
                continue;
            case 7:
                continue;
            case 8:
                printf("Ingrese la ruta a la que desea moverse\n");
                scanf("%c",ruta);
                pfs = MoverA(pfs, ruta);
            case 9:
                printf("Hasta luego\n");
                return 0;
        }
    }    
}
