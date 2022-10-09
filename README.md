Este algoritmo computa a transformação de um container A em um tempo t a partir de uma foto deste mesmo container no tempo t - x. 

Este algoritmo trabalhar com um container especialmente desenvolvido para mapear elementos através do tempo t - x até t, este container

é chamado de Snapshot e está contido no arquivo include/book.h, este container nada mais é que uma foto de um container A que queremos
mapear a sua transformação do tempo t - x até t, sendo assim, os elementos do container C devem ter como base a struct BaseElement,
contido no arquivo BaseElement ou implmentar a propriedade publica int _id. Este _id é o identificador único daquele objeto, sendo 
assim não pode ser repetido. 

Também existe algumas regras com relação ao ciclo de vida de um elemento dentro do container:
1 - Quando um elemento é inserido no container ele recebe um _id único incremental.
2 - Após a remoção de um elemento na posição N os elementos, na posição N + 1 em diante irão mover para N - 1
    porem seus _ids continuam o mesmo;
3 - Caso seja necessário mover um elemento da posição N para uma posição N + X é necessário fazer o seguinte procedimento,
remover o elemento da poisção N (regra 2) e inserir ele novamente na posiçao N + X (regra 3).

As regras acima são fundamentais para mapear os elementos que se mantiveram no container através do tempo já que o algoritmo mapeia a posicão
de cada elemento dentro do container e não o dado contido nele.

```cpp

#include "conflation/container.h"
#include <memory>
#include <atomic>

//Exemplo de uma classe basica 
struct Ordem
{
    int _id;

    Ordem()
    {
        _id = generate_id();
    }

    void Reset()
    {
        _id = generate_id();
    }

    private:
    static std::atomic<int> _count;

    int generate_id()
    {
        return _count.fetch_add(1);
    } 
};

void createSnapshot()
{
    std::vector<Ordem> book;

    book.push_back(...);
    book.push_back(...);
    book.push_back(...);

    auto bs0 = std::make_shared<conflation::Snapshot<Ordem>>(_b0.begin(), _b0.end());
}

```

No código acima temos um exemplo de uma classe que representa um elemento válido 
para ser mapeado 