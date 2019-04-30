#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <assert.h>


#define NB_SP 5

/*-----------------------------------------------------------------------------------------------*/
/*											UPGMA												 */
/*-----------------------------------------------------------------------------------------------*/

typedef struct _un_noeud Un_noeud;

struct _un_noeud {
	
	char *sp;
	Un_noeud *fils_gauche;
	Un_noeud *fils_droit;
	float distG, distD;

};

Un_noeud *CreerNoeud(char *name, Un_noeud *fils_gauche, Un_noeud *fils_droit, float distG, float distD);
int nbFeuilles(Un_noeud *A);


/*
L'ordre de l'algorithme:

*trouver le min dans la matrice
*mettre à jour les arbres
*mettre à jour la matrice
*/



float **copierMatCarre(float **matDist, int n){

	float **newM;

	int i;
	/*Allocation de la nouvelle matrice*/

	newM=malloc(sizeof(float*)*(n));
	for(i=0;i<n;i++){
		newM[i]=malloc(sizeof(float)*(n));
		memcpy(newM[i], matDist[i], sizeof(float)*(n));
	}
	return newM;
}

/***************************************************/
/*	1. allouer le tableau des neouds
	2. créer un noeud pour chaque espèce
		2.0. écrire une fonction creerNoeud()
		2.1. moetre dans le tableau et mettre le nom d'espèce
	3. Retourner le tableau
*/
/***************************************************/

Un_noeud ** creer_les_arbres(char **lesSP,int nb_sp){
    Un_noeud **arbre=malloc(sizeof(Un_noeud*)*nb_sp);
    
    for(int i=0;i<nb_sp;i++){
        arbre[i]=CreerNoeud(lesSP[i],NULL,NULL,0.0,0.0);
    }
    return arbre;
    /*
     Moi
     Allouer le tableau des noeuds
     Pour chaque espece, utiliser la fonction CreerNoeud,
     le mettre dans le tableau et donner nom d'sp
     retourner le tableau
     */
}



/***************************************************/
/*	1. allouer et initialiser TOUS les champs */
/***************************************************/

Un_noeud *CreerNoeud(char *name, Un_noeud *fils_gauche, Un_noeud *fils_droit, float distG, float distD){
    Un_noeud *noeud=malloc(sizeof(Un_noeud));
    noeud->sp=name; /* ? */
    noeud->fils_gauche=fils_gauche;
    noeud->fils_droit=fils_droit;
    noeud->distG=distG;
    noeud->distD=distD;
    return noeud;
    /*
     Moi
     Allouer et initialiser TOUS les champs*/
}

/***************************************************/
/* Trouver la distance minimum dans la matrice
retourner ses indices dans  iMin et  jMin*/
/***************************************************/
void trouverMin(float **mat, int n, int *iMin, int *jMin) {

int i,j;
assert(n>=2);

float min = mat[0][1]; 
*iMin = 0; /*ICI*/
*jMin = 1;
for(i=1; i<n; i++) { /* NON*/
for (j=0; j<i; j++) {
if (mat[i][j] < min) {
min = mat[i][j];
*iMin = i;
*jMin = j;

			}
		}
	}




}

/***************************************************/
/*	-Créer un nouvel arbre qui aura comme fils gauche l'arbre d'indice iMin, et comme fils droit, l'arbre d'indice jMin.
	-iInitialiser les distances distG et distD. On a besoin de nouvel fonction!
	-Créer un nouveau tableau de pointers vers des arbres de taille n-1. Recopier tous les arbres sauf iMin et jMin.
		Ajouter le nouvel arbre en dernier. 
	-Libérer l'ancien tableau, retourner le nouveau.
*/
/***************************************************/

Un_noeud** MAJ_arbres(Un_noeud **tA, int n, int iMin, int jMin, float Dij){
	float DistG = 0.5*Dij, DistD = 0.5*Dij; /*ICI*/
	Un_noeud **Arbre_Intermediaire = malloc(sizeof(Un_noeud*)*(n-1));
	Un_noeud *Nouveau_noeud = CreerNoeud(NULL, tA[iMin], tA[jMin], DistG, DistD);
	Nouveau_noeud->sp=malloc(sizeof(char)*(strlen(tA[iMin] -> sp)+strlen(tA[jMin] -> sp)+1));
	strcpy(Nouveau_noeud->sp, tA[iMin] -> sp);
	strcat(Nouveau_noeud->sp, tA[jMin] -> sp);
	//fprintf(stderr, "Le nouveau noeud a pour id %s\n", Nouveau_noeud->sp);
    int j=0;
	for(int i = 0; i<n; i++){
        if(i != iMin && i != jMin){
            Arbre_Intermediaire[j] = tA[i];
            j++; //on utilise j car Arbre_Intermedaire de taille n-1 et tA de taille n donc j!=i parfois  
        	}
    }
	Arbre_Intermediaire[n-2]=Nouveau_noeud; /*ICI*/
	free(tA);
	return Arbre_Intermediaire;
}



float sommeDist(Un_noeud *A){
	float somme=0;
	if(A!=NULL){somme+=sommeDist(A->fils_gauche);return A->distG;}
	return somme;}

/***************************************************/
/*	-Allouer une nouvelle matrice de taille n-1
	-Recopier les linges et lolonnes sauf iMin et jMin
	-Calculer la nouvelle ligne et colonne et le mettre en dernier
	-Liberer l'ancienne matrice
	-Retourner la matrice
*/
/***************************************************/

float** MAJ_matrice(float** Mat, int iMin, int jMin, int n, Un_noeud *A) {
	float **NMat;
	NMat =(float**) malloc((n-1)*sizeof(float*));
	
	int k,l;
	
	for(k=0; k<(n-1); k++){
		NMat[k] = calloc(sizeof(float),n-1);		
	}
	int i,j;	
	k=0;
	l=0;
 	int ni, nj;
 	ni=nbFeuilles(A->fils_gauche);
 	nj=nbFeuilles(A->fils_droit);

	for(i=0; i<n; i++){
		
 
		if(i!=iMin && i!=jMin ){
		
			for(j=0; j<n; j++){
				if(j!= jMin && j!=iMin){
		
					NMat[k][l++] = Mat[i][j];
				}
			}
			NMat[k][n-2]=((Mat[i][iMin]*ni+Mat[i][jMin]*nj)/(ni+nj));
			NMat[n-2][k]=((Mat[i][iMin]*ni+Mat[i][jMin]*nj)/(ni+nj));
			k++;
			l=0;
		}
	}	
	for(k=0; k<n; k++){
		free(Mat[k]);
		}
	free(Mat);
	return (NMat);
}



/***************************************************/
/*	Retourne le nombre de feuilles de A
*/
/***************************************************/
int nbFeuilles(Un_noeud *A) {
	if(!A){
		return 0;
	}
	if(A->fils_gauche==NULL && A->fils_droit==NULL){
		return 1;
	}
	return nbFeuilles(A->fils_gauche) + nbFeuilles(A->fils_droit);
}

/***************************************************/
/* Ecrire l'arbre au format newick */


void affiche_newick(Un_noeud *A){

    if (A!=NULL){
        if(A->fils_gauche==NULL && A->fils_droit==NULL){
		    printf("%s ",A->sp);
    	}else{        
    		printf("(");
	    	affiche_newick(A->fils_gauche);
			printf(":%f ,", A->distG);
    		affiche_newick(A->fils_droit);
			printf(":%f)", A->distD);

    	}

    }
}

/***************************************************/
/*	-prend le tableau des espèces et la matrice des dist, et n = le nb d'espèces
	-retourne l'arbre
	Créer les arbres
	Repeter n-1 fois:
		*trouver min
		*m.a.j. arbres
		*m.a.j. la matrice*/
/***************************************************/
Un_noeud* UPGMA(char **lesSP, float **matDist, int n) {

	int iMin, jMin;
	/*Creation des n arbres feuilles*/
	Un_noeud **lesAB=creer_les_arbres(lesSP, n), *A;
	/*Copier matDist*/
	float **matDist2=copierMatCarre(matDist, n);



	/* UPGMA*/	
	for(;n >1;n-- ){
		printf("*************Etape %d\n", n);
		for(int i=0;i<n; i++){
			affiche_newick(lesAB[i]);
			printf("\n");
		}
		trouverMin(matDist2, n, &iMin, &jMin);
		lesAB=MAJ_arbres(lesAB,  n,  iMin,  jMin, matDist2[iMin][jMin]);
		matDist2=MAJ_matrice(matDist2, iMin,jMin,n, lesAB[n-2]);
	}

	free(matDist2[0]);
	free(matDist2);
	A=lesAB[0];
	free(lesAB);

	return A;

}

/*******************************************/

int main(){		
	int i;
	float ex_matDist[NB_SP][NB_SP]={{0,0.092,0.106,0.177,0.207},{0.092,0,0.111,0.193,0.218}, {0.106 ,0.111 ,0,0.188,0.218},{0.177,0.193,0.188,0,0.219},{0.207 , 0.218	,0.218	,0.219,0}};
	float *ex_matDist2[NB_SP];
	char *lesSP[NB_SP]={"H","C","Go","O", "Gi"};
	Un_noeud  *pA;


	/*matDist est de type float [][], il faut le transformer en tableau de type float ***/
	for(i=0;i<NB_SP;i++)
		ex_matDist2[i]=&(ex_matDist[i][0]);


	pA=UPGMA(lesSP, ex_matDist2, NB_SP);

	affiche_newick(pA);
	printf("\n");
	return 0;
}
















