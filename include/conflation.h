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
				/*
				//se a ordem value b1 estiver contida em b0 
				//e sua posição atual for maior que relação
				//a anterior signifca que ocorreu alguns inserts
				//então deve-se calcular o delta entre as duas 
				//posições, se o delta for igual a variável inserts
				//significa que este delta será ajustado automaticamente
				//após os envios dos comandos inserts
				//se a diferença entre o delta e o os inserts  gerar
				//uma diferença estas posições precisam ser removidas
				//value2->positon até value2->position - |delta - inserts| 
				else if(delta > 0)
				{
					auto remove = delta - inserts;
					if(remove)
					{
						
						ret.emplace_back(container::make_command(container::OPERATION::DELETE_THRU,
									value2->position + b0_delta - remove,
									value2->position + b0_delta));
						b0_delta -= remove;
									
					}
				}
				*/
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
