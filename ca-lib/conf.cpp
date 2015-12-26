#include "conf.h"

/* 
Aloca na mem�ria a estrutura 'Configuration' que armazena informa��es sobre
o reticulado do aut�mato celular de uma dimens�o. A fun��o retorna um
ponteiro para a estrutura criada. Caso ocorra algum erro de aloca��o de
mem�ria, o valor retornado � 'nullptr'.
Par�metros:
  - radius: Indica o raio de vizinhan�a dos s�tios no reticulado;
  - char_map: Uma 'const string' usada para imprimir os estados no diagrama
    espa�o-temporal do aut�mato celular;
  - size: Informa o tamanho do at�mato celular, isto �, a quantidade de s�tios
    em seu reticulado.
*/
Configuration * create_configuration(const uint_t radius,
	const uint_t size,
	const char_state_t char_map[],
	const uint_t char_map_size) {

	Configuration * new_conf = new Configuration;
	if (!new_conf) 
		return nullptr;
	new_conf->size = size;
	new_conf->radius = radius;
	new_conf->periodic_sites = new state_t[size + 2 * radius];
	if (!new_conf->periodic_sites) {
		delete new_conf;
		return nullptr;
	}
	new_conf->first_site = new_conf->periodic_sites + radius;
	new_conf->last_site = new_conf->first_site + size;
	new_conf->char_map = new char_state_t[char_map_size + 1];
	strcpy_s(new_conf->char_map, char_map_size + 1, char_map);
	new_conf->char_map_size = char_map_size;
	return new_conf;
}

/*
Rotina que remove da mem�ria as vari�veis alocadas em 'create_configuration'.
Par�metro:
  - *conf: Um ponteiro para uma estrutura 'Configuration' criada previamente.
*/
void destroy_configuration(Configuration * conf) {
	delete conf->periodic_sites;
	delete conf->char_map;
	delete conf;
	conf = nullptr;
}

/*
Rotina que aumenta a quantidade de s�tios em um reticulado de uma estrutura 
'Configuration'. Os novos s�tios s�o alocados na mem�ria e as informa��es
da estrutura atualizada. Se houver um erro no processo de aloca��o de mem�ria
a fun��o retorna 'false', caso contr�rio retorna 'true'.
Par�metros:
  - *conf: Ponteiro para uma estrutura 'Configuration';
  - increase: Tamanho a ser acrescentado no reticulado;
  - char_state: Caractere de base "0" do estado dos s�tios a serem acrescidos. 
*/
check_return append_sites(Configuration * conf, 
	const uint_t increase, 
	const char_state_t char_state) {

	state_t* new_periodic_border = new state_t[conf->size + 2 * conf->radius + increase];
	if (!new_periodic_border) 
		return false;
	for (uint_t i = conf->radius; i < conf->size + conf->radius; ++i)
		new_periodic_border[i] = conf->periodic_sites[i];
	for (uint_t i = conf->size + conf->radius; i < conf->size + conf->radius + increase; ++i)
		new_periodic_border[i] = static_cast<state_t>(char_state - ASCII_TO_UCHAR_CONF);
	delete conf->periodic_sites;
	conf->periodic_sites = new_periodic_border;
	conf->first_site = conf->periodic_sites + conf->radius;
	conf->size += increase;
	conf->last_site = conf->first_site + conf->size;
	update_periodic_border(conf);
	return true;
}

/*
Copia os dados da estrutura 'Configuration' indicada em 'conf_from' para 'conf_to',
incluindo s�tios, tamanho, raio. Se houver um erro no processo de aloca��o de mem�ria
a fun��o retorna 'false', caso contr�rio retorna 'true'.
Par�metros:
  - *conf_to: Ponteiro para a estrutura 'Configuration' para a qual ser�o copiados os
       dados;
  - *conf_from: Ponteiro para a estrutura 'Configuration' da qual os dados ser�o
       copiados.
*/
check_return copy_configuration(Configuration * conf_to, 
	const Configuration * conf_from) {

	if (!conf_to || (conf_to->size != conf_from->size)) {
		Configuration * new_conf = new Configuration;
		new_conf->size = conf_from->size;
		new_conf->radius = conf_from->radius;
		new_conf->periodic_sites = new state_t[new_conf->size + 2 * new_conf->radius];
		if (!new_conf->periodic_sites) {
			delete new_conf;
			return false;
		}
		new_conf->first_site = new_conf->periodic_sites + new_conf->radius;
		new_conf->last_site = new_conf->first_site + new_conf->size;
		destroy_configuration(conf_to);
		conf_to = new_conf;
	}
	for (uint_t i = 0; i < conf_from->size + 2 * conf_from->radius; ++i)
		conf_to->periodic_sites[i] = conf_from->periodic_sites[i];
	return true;
}

/*
Embaralha aleatoriamente uma dada configuracao.
Par�metro:
  - *conf: Ponteiro para uma estrutura 'Configuration' que ter� os estados dos
       s�tios embaralhados.
*/
void shuffle_configuration(Configuration * conf) {

	if (conf->size < 1)
		return;
	for (uint_t i = conf->size - 1; i > 1; --i) {
		uint_t random_displacement = jkiss_generator(conf->size);
		state_t* exchange_site = conf->first_site + random_displacement;
		state_t exchange_state = *exchange_site;
		*exchange_site = conf->first_site[i];
		conf->first_site[i] = exchange_state;
	}
	update_periodic_border(conf);
}

/*
Atualiza os estados de borda de acordo com os estados dos s�tios do reticulado.
A quantidade de s�tios de borda depende do tamanho do raio de vizinhan�a.
Par�metro:
  - *conf: Ponteiro para uma estrutura 'Configuration' que ter� os estados dos
       s�tios de sua borda peri�dica atualizados.
*/
void update_periodic_border(Configuration * conf) {

	if (conf->size < 1)
		return;
	for (uint_t i = 0; i < conf->radius; ++i) {
		conf->periodic_sites[i] = conf->last_site[(int)(i - conf->radius)];
		conf->periodic_sites[conf->radius + conf->size + i] = conf->first_site[i];
	}
}

/*
Conta os s�tios que estiverem em um determinado estado.
Par�metros:
  - *conf: Ponteiro para uma estrutura 'Configuration' que ter� os estados dos
       s�tios contados;
  - state: estado a ser contado.
*/
uint_t count_conf_state(Configuration * conf,
	const state_t state) {

	uint_t result = 0;
	for (state_t* site = conf->first_site; site != conf->last_site; ++site)
		result += (*site == state);
	return result;
}

/*
Verifica se uma configura��o � igual � outra. Compara a propriedade 'size',
em seguida verifica cada um dos estados dos s�tios de ambas configura��es.
Par�metros:
  - *conf1: Ponteiro para uma estrutura 'Configuration' que ter� os estados dos
       s�tios verificados com a configura��o apontada em '*conf2';
  - *conf2: Ponteiro para uma estrutura 'Configuration' que ter� os estados dos
	   s�tios verificados com a configura��o apontada em '*conf1'.
*/
bool are_equal_configurations(const Configuration * conf1, 
	const Configuration * conf2) {

	if (conf1->size != conf2->size) return false;
	for (uint_t i = 0; i < conf1->size; ++i)
		if (conf1->first_site[i] != conf2->first_site[i]) return false;
	return true;
}

/*
Altera aleatoriamente os estados dos s�tios de uma dada configura��o numa
quantidade predeterminada.
Par�metros:
  - *conf: Ponteiro para uma estrutura 'Configuration' que ter� os estados dos
       s�tios alterados;
  - damage_size: Quantidade de s�tios que ter�o seu estado alterado;
  - *damaging_char_states: Ponteiro para uma string de caracteres que informa
       para quais estados ser�o alterados os s�tios. Os estados aqui informados,
	   devem ter como base o caractere ASCII '0' (0x20) equivalente ao estado 0 e
	   assim sucessivamente;
  - damaging_char_states_size: � o tamanho da string 'damaging_char_states'.
*/
void damage_configuration(Configuration * conf,
	uint_t damage_size,
	const char_state_t* damaging_char_states,
	const uint_t damaging_char_states_size) {

	if (damage_size > conf->size) damage_size = conf->size;
	state_t * damaged_array = new state_t[damage_size];
	for (uint_t i = 0; i < damage_size; ++i)
		damaged_array[i] = static_cast<state_t>(damaging_char_states[jkiss_generator(damaging_char_states_size)] - ASCII_TO_UCHAR_CONF);
	for (uint_t i = 0; i < damage_size; ++i)
		conf->first_site[i] = damaged_array[i];
	delete[] damaged_array;
	shuffle_configuration(conf);
}
/*
Fun��o de sobrecarga de operador que imprime uma dada configura��o informada em 'conf'
em um 'stream' informado. Exemplo de uso: 
	Configuration * conf = create_configuration(1, "0123", 0);
	append_sites(conf, 80, '0');
	std::cout << *conf;
*/
std::ostream& operator<<(std::ostream& stream, 
	const Configuration& conf) {

	for (uint_t i = 0; i < conf.size; ++i)
		stream << conf.char_map[conf.first_site[i]];
	return stream;
}
