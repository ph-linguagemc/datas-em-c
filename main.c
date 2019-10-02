#include <stdio.h>
#include <stdlib.h>
#include <string.h >

#define DATA_LEN 10
#define BARRA1 2
#define BARRA2 5
#define MIN_ANO 1
#define MAX_ANO 2500
#define DIAS_SEM 7 
#define DIAS_ANO 365
#define QTD_MES 12

//#define DEBUG

#ifdef DEBUG
	#define DPRINTF(fmt, args...) fprintf(stderr,"%s %s " fmt,__FILE__, __FUNCTION__, ##args )
#else
	#define DPRINTF(fmt, args...) 
#endif

int DataToInt( char *data );
char* IntToData( int tot_dias, char *data );
int Bissexto( int ano );
int DataValida( char *data );
int GetAno( char *data );
int GetMes( char *data );
int GetDia( char *data );
int IsDigit( char letra );
int DataValida( char *data, int *dia, int *mes, int *ano, int *bis  );
int DiaSemana( char *data );
int DiasEntreDatas(char *data1, char *data2 );
char* SomaDataDias(char *data1, char *data2, int dias );
 

const int dias_mes[2][QTD_MES]={
{31,28,31,30,31,30,31,31,30,31,30,31},
{31,29,31,30,31,30,31,31,30,31,30,31}
};

const char *nome_dia[DIAS_SEM+1]={"data invalida","sabado","domingo","segunda","terça","quarta","quinta","sexta"};

int IsDigit( char letra )
{
	DPRINTF("letra=%c\n", letra );
	return ( letra >= '0' && letra <= '9');
}

int DataValida( char *data, int *dia, int *mes, int *ano, int *bis  )
{
	int i;
	
	if ( strlen(data)!= DATA_LEN )
	{
		DPRINTF( "Data com tamanho errado, use: dd/mm/aaaa\n");
		return 0;
	}
	
	if ( data[BARRA1]!='/' || data[BARRA2]!='/' )
	{
		DPRINTF( "Separador invalido, use: dd/mm/aaaa\n");
		return 0;	
	}
	
	for ( i=0; i<DATA_LEN; i++ )
	{
		if ( i==BARRA1 || i==BARRA2 ) continue;
		if ( !IsDigit( data[i]) )
		{
			DPRINTF("Caracter invalido: %c na data na posicao %d\n",data[i], i );
			return 0;		
		}
	
	}
	
	*ano = GetAno(data);
	*mes = GetMes(data);
	*dia = GetDia(data);
	*bis = Bissexto(*ano);
	
	DPRINTF("dia=%d mes=%d ano=%d bissexto=%d\n", *dia, *mes, *ano, *bis );
	if ( *ano < MIN_ANO || *ano > MAX_ANO )
	{
		DPRINTF("Ano invalido: %d\n", *ano );
		return 0;		
	}
	
	if ( *mes < 1 || *mes > QTD_MES )
	{
		DPRINTF("Mes invalido: %d\n", *mes );
		return 0;		
	}

	if ( *dia < 1 || *dia > dias_mes[*bis][*mes - 1] )
	{
		DPRINTF("Dia invalido: %d\n", *dia );
		return 0;		
	}

	
	return 1;
}

int GetAno( char *data ) //formato dd/mm/aaaa
{
	char str[5];
	
	str[0]=data[6];
	str[1]=data[7];
	str[2]=data[8];
	str[3]=data[9];
	str[4]=0;
	
	return atoi(str);
}

int GetMes( char *data ) //formato dd/mm/aaaa
{
	char str[3];
	
	str[0]=data[3];
	str[1]=data[4];
	str[2]=0;
	
	return atoi(str);
}


int GetDia( char *data ) //formato dd/mm/aaaa
{
	char str[3];
	
	str[0]=data[0];
	str[1]=data[1];
	str[2]=0;
	
	return atoi(str);
}

char* IntToData( int tot_dias, char *data )
{
	int i, total, ano, mes, dia, bis;
	
	
	total = tot_dias;
	
	dia   = 1;
	mes   = 1;
	ano   = 1;
	bis   = 0;
	
	while ( total > (DIAS_ANO+bis) )
	{
		total -= (DIAS_ANO+bis);
		ano++;
		bis = (( ano % 4 )==0);	
	}
	 	
	for ( i=0; i<QTD_MES; i++ )
	{
		if ( ( total - dias_mes[bis][i] ) > 0 )
		{	
			total -= dias_mes[bis][i];
			mes = i+2;
			DPRINTF("Subtraindo mes[%2d]=%d ==> total=%d\n",i+1,dias_mes[bis][i], total );
		}
		else
		{
			break;
		}
	}
	
	dia = total;
	
	DPRINTF("dia=%d mes=%d ano=%d\n", dia, mes, ano );
	
	sprintf(data,"%02d/%02d/%04d", dia, mes, ano );
	
	DPRINTF("data2= %s\n",data);
	
	return data;
		
}

int DataToInt( char *data )
{
	int dia, mes, ano, bis, tot_dias;
	int i, a, b;
	
	tot_dias = 0;
	
	
	if ( DataValida(data,&dia,&mes,&ano,&bis) )
	{
		a = ano / 4;
		b = ano % 4;
		
		if ( b )
		{
			tot_dias = DIAS_ANO * (ano-1) + a;
			DPRINTF("Passo1 dias=%d\n", tot_dias );
		}
		else
		{
			tot_dias = DIAS_ANO * (ano-1) + a-1;
			DPRINTF("Passo2 dias=%d\n", tot_dias );	
		}
		for ( i=0; i<QTD_MES; i++ )
		{
			if ( i+1 == mes )break;
			tot_dias+= dias_mes[bis][i];
			DPRINTF("Somando mes[%2d]=%d ==> tot_dias=%d\n",i+1,dias_mes[bis][i], tot_dias );
		}
		DPRINTF("Passo3 dias=%d\n", tot_dias );
		tot_dias += dia;
		DPRINTF("Somando dia=%d\n", dia );
		DPRINTF("Passo4 tot_dias=%d\n", tot_dias );
		return tot_dias;
	}
	
	return 0;
}

int Bissexto( int ano )
{
	if ((ano % 400)== 0 ) return 1;
	if (((ano % 4)==0 ) && ((ano % 100) != 0 ) )
		return 1;
	return 0;
}

int DiaSemana( char *data )
{
	int tot_dias;
	
	tot_dias = DataToInt(data);
	
	if ( tot_dias )
	{
		return (tot_dias % DIAS_SEM+1);
	}
	return 0; 
}
int DiasEntreDatas(char *data1, char *data2 )
{
	int tot_dias1, tot_dias2;
	
	tot_dias1 = DataToInt(data1);
	tot_dias2 = DataToInt(data2);
	
	if ( tot_dias1 && tot_dias2 )
	{
		return tot_dias2 - tot_dias1;
	}
	DPRINTF("Data invalida\n");
	return 0;
}

char* SomaDataDias(char *data1, char *data2, int dias )
{
	int tot_dias;
	
	tot_dias = DataToInt(data1);
	if (!tot_dias)
	{
		DPRINTF("Data invalida: %s\n", data1 );
		return 0;
	}
	tot_dias += dias;
	
	return IntToData(tot_dias,data2);
	
}
 
int main(int argc, char** argv) 
{
	int tot_dias, dia_sem;
	char data1[DATA_LEN+1];
	char data2[DATA_LEN+1];
	
	strcpy(data1,"13/07/1966");
	
	tot_dias = DataToInt(data1);
	printf("Data %s tot_dias=%d\n", data1, tot_dias );
	
	dia_sem = DiaSemana(data1);
	printf("Data %s dia_sem=%d (%s)\n",data1, dia_sem, nome_dia[dia_sem]);
	
	
	IntToData(tot_dias,data2);
	printf("dias=%d corresponde data=%s\n",tot_dias, data2);
	
	tot_dias=DiasEntreDatas(data1,data2);
	
	strcpy(data1,"13/07/1966");
	strcpy(data2,"24/09/2019");
	
	tot_dias = DiasEntreDatas(data1,data2);
	
	printf("Entre %s e %s sao %d dias\n", data1, data2, tot_dias );
	
	SomaDataDias(data1, data2, tot_dias );
	
	printf( "Data %s somando %d dias da data %s\n", data1, tot_dias, data2 );
	
	SomaDataDias(data2, data1, -tot_dias );
	
	printf( "Data %s somando %d dias da data %s\n", data2, -tot_dias, data1 );
		
	return 0;
}
