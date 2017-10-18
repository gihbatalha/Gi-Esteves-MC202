//Estrturas

//Métodos das estruturas

//Métodos relacionados ao S.O.

//Métodos que complementam a main
void preparaParaIniciarProcesso(){
	//Ler as variáveis necessárias para essa operação
	//Chamar os métodos necessários
}

void preparaParaFinalizarProcesso(){
	//Ler as variáveis necessárias para essa operação
	//Chamar os métodos necessários
}

void preparaParaCalcularFragmentacao(){
	//Ler as variáveis necessárias para essa operação
	//Chamar os métodos necessários
}

void preparaParaGerarRelatorioDoSistema(){
	//Ler as variáveis necessárias para essa operação
	//Chamar os métodos necessários
}

void preparaParaImprimirSementes(){
	//Ler as variáveis necessárias para essa operação
	//Chamar os métodos necessários
}

void preparaParaImprimirProcessos(){
	//Ler as variáveis necessárias para essa operação
	//Chamar os métodos necessários
}



int main(){

	int op;

	while(scanf("%d", &op) != EOF){
		switch(op){
			case 1:	preparaParaIniciarProcesso();
					break;
			case 2:	preparaParaFinalizarProcesso();
					break;
			case 3: preparaParaCalcularFragmentacao();
					break;
			case 4:	preparaParaGerarRelatorioDoSistema();
					break;
			case 5:	preparaParaImprimirSementes();
					break;
			case 6:	preparaParaImprimirProcessos();
					break;
		}
	}	
}