#ifndef __CONFLATION__
#define __CONFLATION__

#include<memory>
#include<functional>
#include<vector>
#include"container.h"

namespace conflation
{
	template<class T>
	static std::vector<container::command> compute_diff(const std::shared_ptr<container::Snapshot<T>>& b0, 
							const std::shared_ptr<container::Snapshot<T>>& b1)
	{
		int inserts = 0;
		int b0_delta = 0;
		std::vector<container::command> ret;

		for(const auto& value : *b1)
		{
			auto value2 = b0->find(value);

			if(value2)
			{
				//NOTA: b0_delta é a soma de todos os deltas ocorridos na operação
				// isto que dizer que a cada posição analisada o algoritmo assume que
				// o container já está organizado da posição 1 até p - 1
				// sendo assim com a soma b0_delta o valor de delta nunca será 
				// maior que 0. ainda necessito provar matemáticamente.
				// porem por empirismo até o momento o delta nunca foi maior que 0 
				// em todas as combinações possíveis .
				auto delta = value->position - (value2->position + b0_delta);
				
				//se a ordem value b1 estiver contida em b0
				//e sua posição atual for menor em relação 
				//ao book b0 então o delta entre as duas ordens
				//precisa ser removidos
				if (delta < 0)
				{
					
					ret.emplace_back(container::make_command(container::OPERATION::DELETE_THRU,
									value2->position + delta + b0_delta,
									value2->position + b0_delta));

					b0_delta += delta;
									
				}
				
				inserts = 0;
			}
			else
			{
				
				ret.emplace_back(container::make_command(container::OPERATION::INSERT,
							value->position, 0, value->id));
							
				++inserts;	
				++b0_delta;
			}
		}
		if((b0->size() + b0_delta) > b1->size())
		{
			ret.emplace_back(container::make_command(container::OPERATION::DELETE_THRU,
						b1->size() +  1,
						b0->size() + b0_delta + 1));
		}
		return ret;
	}

};
#endif
