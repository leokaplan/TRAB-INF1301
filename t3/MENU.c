#include "MENU.h"
#include "GRAFO.h"
#include <stdlib.h>
typedef struct Menu_{
    char* nome;
    int id;
    GRA_tppGrafo opcoes;        
    int pai;
} Menu;
typedef struct Opcao_{
    char cmd;
    char* nome;
    void (*callback)(EST_tppEstado e,MEN_tppOpcao o);
} Opcao;

void MEN_DestruirMenu(MEN_tppMenu menu){
    LIS_DestruirLista(menu->opcoes);
    free(menu);
}
/*

   Por hipotese existe o grafo menus, id identifica, nome!=null

 */
//mudar pra usar grafo em vez de lista
void volta(EST_tppEstado e,MEN_tppOpcao o){
    
}

MEN_tpCondRet MEN_CriarMenu(MEN_tppMenus menus, int id, char* nome,int idpai){
    MEN_tppMenu m = malloc(sizeof(Menu));
    if(m==NULL)
        return MEN_CondRetFaltouMemoria;
    m->opcoes = LIS_CriarLista(free);         
    if(m->opcoes==NULL){
        free(m);
        return MEN_CondRetFaltouMemoria;
    }
    m->nome = nome;
    m->id = id;
    GRA_InserirVertice(menus,m,m->id);
    //ifs e returns...
    //TODO:Volta interno
    MEN_tpCondRet cr = MEN_CriarOpcao(menus, m->id,'0', "Ir para o menu acima (encerrar o programa caso o menu atual seja o inicial(Inicio))",volta);
    if(cr!=MEN_CondRetOK)
    {
        LIS_ExcluirLista(m->opcoes);
        free(m);
    }
    return cr;
}

//mudar lis->gra
MEN_tpCondRet MEN_CriarOpcao(MEN_tppMenus menus, int idMenu,char cmd, char* nome,void (*callback)(EST_tppEstado e,MEN_tppOpcao o)){
    MEN_tppOpcao o = malloc(sizeof(Opcao));
    if(o==NULL)
        return MEN_CondRetFaltouMemoria;
    //if(nome==NULL)
    //return erro;
    o->nome = nome;
    o->cmd = cmd;
    o->callback = callback;
    MEN_tppMenu m;
    GRA_ObterValor(menus,idMenu,m);
    LIS_InserirElementoApos(m->opcoes,o);
    return MEN_CondRetOK;
}
MEN_tpCondRet MudaMenu(MEN_tppMenus menus,int id){
    	
    GRA_IrVizinho(menus,id);
}
