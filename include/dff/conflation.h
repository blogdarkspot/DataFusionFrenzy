/***
MIT License

Copyright (c) 2022 Fábio Santana fss.dsv.src@gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
**/
#ifndef __DFF_ALGO_CONFLATION__
#define __DFF_ALGO_CONFLATION__

#include<memory>
#include<functional>
#include<vector>
#include"container.h"

namespace dff
{
namespace algo
{
	template<class T>
	static std::vector<container::command<T>> compute_diff(const std::shared_ptr<container::Snapshot<T>>& b0, 
							const std::shared_ptr<container::Snapshot<T>>& b1)
	{
		int inserts = 0;
		int b0_delta = 0;
		std::vector<container::command<T>> ret;

		for(const auto& value : *b1)
		{
			auto value2 = b0->find(value);

			if(value2)
			{
				//NOTA: b0_delta é a soma de todos os deltas ocorridos na operação
				// isto que dizer que a cada posição analisada o algoritmo assume que
				// o container já está organizado da posição 1 até p - 1
				// sendo assim com a soma b0_delta o valor de delta nunca será 
				auto delta = value->position - (value2->position + b0_delta);
				
				//se a ordem value b1 estiver contida em b0
				//e sua posição atual for menor em relação 
				//ao book b0 então o delta entre as duas posições 
				//precisam ser removidos
				if (delta < 0)
				{
					
					ret.emplace_back(container::make_command<T>(container::OPERATION::DELETE_THRU,
									value2->position + delta + b0_delta,
									value2->position + b0_delta));

					b0_delta += delta;
									
				}
				
				inserts = 0;
			}
			else
			{
				
				ret.emplace_back(container::make_command<T>(container::OPERATION::INSERT,
							value->position, 0, value->id));
							
				++inserts;	
				++b0_delta;
			}
		}
		if((b0->size() + b0_delta) > b1->size())
		{
			ret.emplace_back(container::make_command<T>(container::OPERATION::DELETE_THRU,
						b1->size() +  1,
						b0->size() + b0_delta + 1));
		}
		return ret;
	}

};
};
#endif
