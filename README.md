- Mudar o jeito de câmera, ao invés de mudar a viewport, fixar a vierport na câmera e mover somente a câmera
- Centralizar a câmera no personagem
- Mudar o modo de visualização para perspectiva e ir mudando a posição da câmera para ver a arena achatada

# Requisitos
- [x] A janela de visualização deverá ter 500x500 pixels
- [ ] Carregar arena agora com com a coordenada z com metade do tamanho da y
- [ ] Personagens devem ser iniciados na coordenada x,y fornecida no arquivo svg e coordenada z aleatória (desde que dentro da arena)
- [ ] Desenhar o jogador (e oponentes) em 3D (usar criatividade)
- [ ] O retângulo de colisão agora é um cilindro
- [ ] A colisão também deve ser calculada para as novas paredes da arena (que agora tem um formato de paralelepípedo, ou um corredor)
'a' e 'd' giram o jogador no sentido horário e anti-horário (respectivamente), enquanto as teclas 'w' e 's' servem para mover o jogador para frente e para trás (respectivamente)
- [ ] O pulo agora deve ser feito com a barra de espaço
- [ ] O braço pode fazer o movimento de um cone imaginário e a bala deve seguir na direção em que a arma estiver apontando
- [ ] O jogo deverá conter pelo menos um modelo de luz na arena (pontual ou direcional)
- [ ] O jogo deverá ter um modo noturno (fazer a troca de modos com a tecla “n”) em que todas as luzes da arena são apagadas, sendo ligado somente uma lanterna na arma (braço) do jogador apontando na mesma direção da arma (A lanterna será representada por uma iluminação spot que deverá ser vista na arena quando iluminada)
- [ ] As paredes, o chão e o teto da arena deverão ser texturizados, assim como o jogador
- [ ] O botão 1 (opção padrão) deverá acionar uma câmera perspectiva posicionada no olho do jogador e olhando para frente. 
- [ ] O botão 2 deverá acionar uma câmera na arma do jogador, ou seja, como se fosse uma mira. Ela será uma câmera perspectiva posicionada em cima da arma, olhando na direção do tiro (paralelamente ao cano da arma) e com o up perpendicular a arma. Com essa visão, seria possível ver parte da arma, assim como o que estiver a frente dela.
- [ ] Você poderá entrar temporariamente na câmera 2 (a partir de qualquer outra) enquanto o botão direito do mouse estiver pressionado. 
- [ ] O botão 3 deverá acionar uma câmera perspectiva posicionada inicialmente atrás do jogador (a uma distância grande suficiente para ver todo o jogador por uma terceira pessoa) e a uma altura superior à do jogador, e olhando para o centro do jogador (up apontando para o teto). 
- [ ] A câmera 3 poderá ser rotacionada (360 graus em torno do jogador e ±60 graus para cima e para baixo do centro do jogado) quando pressionada a tecla x, resultando em um movimento esférico em volta do jogador.
- [ ] As teclas de + e – devem controlar o zoom da câmera 3.
- [ ] Incluir um arquivo README.txt junto com o código fonte do jogo explicando os atalhos e funcionalidades adiconais. 