# Requisitos

## Jogo

- [x] A janela de visualização deverá ter 500x500 pixels
- [x] Jogador deve ganhar ao chegar ao final
- [x] Jogador deve perder ao encostar nos tiros

## Jogador

- [x] Desenhar o jogador em 3D (usar criatividade)
- [x] O retângulo de colisão agora é um cilindro
- [x] 'a' e 'd' giram o jogador no sentido horário e anti-horário (respectivamente), enquanto as teclas 'w' e 's' servem para mover o jogador para frente e para trás (respectivamente)
- [x] A colisão também deve ser calculada para as novas paredes da arena (que agora tem um formato de paralelepípedo, ou um corredor)
- [x] O pulo agora deve ser feito com a barra de espaço
- [ ] O jogador pode apenas se movimentar para frente e para trás enquanto pula
- [x] O braço pode fazer o movimento de um cone imaginário e a bala deve seguir na direção em que a arma estiver apontando

## Oponentes 

- [x] Desenhar o oponente em 3D (usar criatividade)
- [x] Oponente deve se movimentar aleatoriamente
- [ ] Adaptar o movimento para ser em XZ e não apenas em X (confirmar isso)
    - ja tem um prototipo disso comentado no codigo
- [x] Oponente deve atirar de tempos em tempos

## Arena

- [x] Carregar arena normalmente para x e y e com a coordenada z com metade do tamanho da metade y
- [x] Personagens devem ser iniciados na coordenada x, y fornecida no arquivo svg e coordenada z aleatória (desde que dentro da arena)
- [x] O jogo deverá conter pelo menos um modelo de luz na arena (pontual ou direcional)
- [x] O jogo deverá ter um modo noturno (fazer a troca de modos com a tecla “n”) em que todas as luzes da arena são apagadas, sendo ligado somente uma lanterna na arma (braço) do jogador apontando na mesma direção da arma (A lanterna será representada por uma iluminação spot que deverá ser vista na arena quando iluminada)
- [x] As paredes, o chão e o teto da arena deverão ser texturizados
- [x] O jogador deve ser texturizado

## Camera

- [x] O botão 1 (opção padrão) deverá acionar uma câmera perspectiva posicionada no olho do jogador e olhando para frente.
- [x] O botão 2 deverá acionar uma câmera na arma do jogador, ou seja, como se fosse uma mira. Ela será uma câmera perspectiva posicionada em cima da arma, olhando na direção do tiro (paralelamente ao cano da arma) e com o up perpendicular a arma. Com essa visão, seria possível ver parte da arma, assim como o que estiver a frente dela.
- [x] Você poderá entrar temporariamente na câmera 2 (a partir de qualquer outra) enquanto o botão direito do mouse estiver pressionado. 
- [x] O botão 3 deverá acionar uma câmera perspectiva posicionada inicialmente atrás do jogador (a uma distância grande suficiente para ver todo o jogador por uma terceira pessoa) e a uma altura superior à do jogador, e olhando para o centro do jogador (up apontando para o teto). 
- [x] A câmera 3 poderá ser rotacionada (360 graus em torno do jogador e ± 60 graus para cima e para baixo do centro do jogado) quando pressionada a tecla x, resultando em um movimento esférico em volta do jogador.
- [x] As teclas de + e – devem controlar o zoom da câmera 3.
- [x] Incluir um arquivo README.txt junto com o código fonte do jogo explicando os atalhos e funcionalidades adiconais.