# Requisitos

## Jogo

- [x] A janela de visualização deverá ter 500x500 pixels
- [x] Jogador deve ganhar ao chegar ao final
- [ ] Jogador deve perder ao encostar nos tiros ou nos oponentes

## Jogador

- [ ] Desenhar o jogador em 3D (usar criatividade)
- [ ] O retângulo de colisão agora é um cilindro
- [ ] 'a' e 'd' giram o jogador no sentido horário e anti-horário (respectivamente), enquanto as teclas 'w' e 's' servem para mover o jogador para frente e para trás (respectivamente)
- [ ] A colisão também deve ser calculada para as novas paredes da arena (que agora tem um formato de paralelepípedo, ou um corredor)
- [x] O pulo agora deve ser feito com a barra de espaço
- [ ] O jogador pode apenas se movimentar para frente e para trás enquanto pula
- [ ] O braço pode fazer o movimento de um cone imaginário e a bala deve seguir na direção em que a arma estiver apontando
- [ ] (BONUS) Utilizar modelos avançados de jogador e suas partes. Referencia: https://www.mixamo.com/
- [ ] (BONUS) Para quem for usar modelos avançados, será permitido usar a arma acoplada ao ombro para não ter que movimentar o braço do modelo. Neste caso, as funcionalidades referentes ao braço descritas nos itens anteriores deverão ser implementadas para a arma (ou seja, controle com o mouse, atirar, etc).

## Oponentes 

- [ ] Desenhar o oponente em 3D (usar criatividade)
- [ ] Oponente deve se movimentar aleatoriamente
- [ ] Oponente deve atirar de tempos em tempos

## Arena

- [x] Carregar arena normalmente para x e y e com a coordenada z com metade do tamanho da metade y
- [x] Personagens devem ser iniciados na coordenada x, y fornecida no arquivo svg e coordenada z aleatória (desde que dentro da arena)
- [ ] O jogo deverá conter pelo menos um modelo de luz na arena (pontual ou direcional)
- [ ] O jogo deverá ter um modo noturno (fazer a troca de modos com a tecla “n”) em que todas as luzes da arena são apagadas, sendo ligado somente uma lanterna na arma (braço) do jogador apontando na mesma direção da arma (A lanterna será representada por uma iluminação spot que deverá ser vista na arena quando iluminada)
- [ ] As paredes, o chão e o teto da arena deverão ser texturizados, assim como o jogador
- [ ] (BONUS) Deve ter um minimapa com a visão 2D da arena igual no T2D, com um circulo verde p o player e vermelho para os oponentes. Deve ser transparente. O  mapa  deve  ficar  fixo  no  canto  inferior  direito  e  ocupar  1/4  da  largura  da  janela  (ou 
seja, 1/16 da área).

## Camera

- [ ] O botão 1 (opção padrão) deverá acionar uma câmera perspectiva posicionada no olho do jogador e olhando para frente. 
- [ ] O botão 2 deverá acionar uma câmera na arma do jogador, ou seja, como se fosse uma mira. Ela será uma câmera perspectiva posicionada em cima da arma, olhando na direção do tiro (paralelamente ao cano da arma) e com o up perpendicular a arma. Com essa visão, seria possível ver parte da arma, assim como o que estiver a frente dela.
- [x] Você poderá entrar temporariamente na câmera 2 (a partir de qualquer outra) enquanto o botão direito do mouse estiver pressionado. 
- [x] O botão 3 deverá acionar uma câmera perspectiva posicionada inicialmente atrás do jogador (a uma distância grande suficiente para ver todo o jogador por uma terceira pessoa) e a uma altura superior à do jogador, e olhando para o centro do jogador (up apontando para o teto). 
- [x] A câmera 3 poderá ser rotacionada (360 graus em torno do jogador e ± 60 graus para cima e para baixo do centro do jogado) quando pressionada a tecla x, resultando em um movimento esférico em volta do jogador.
- [x] As teclas de + e – devem controlar o zoom da câmera 3.
- [ ] Incluir um arquivo README.txt junto com o código fonte do jogo explicando os atalhos e funcionalidades adiconais.