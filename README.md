# passwordGame
Jogo embarcado em uC Atmega328 (Arduino)

INSTITUTO FEDERAL DE EDUCAÇÃO, CIÊNCIA E TECNOLOGIA DE SANTA CATARINA
DEPARTAMENTO ACADÊMICO DE ELETRÔNICA
CURSO DE ENGENHARIA ELETRÔNICA

ATIVIDADE PRÁTICA
JOGO DA SENHA
1 OBJETIVO
Desenvolver um jogo da senha utilizando um temporizador do ATmega328, um display de cristal líquido alfanumérico 16x2, dois botões de contato rápido e um buzzer.

2 REQUISITOS DE HARDWARE
O sistema deve possuir três botões de contato rápido, um display de cristal líquido alfanumérico e um buzzer. O arquivo de simulação do Proteus é apresentado na sequência e está disponível para download no Moodle.

2.1 REQUISITOS DE SOFTWARE
• O sistema deve ser programado para o microcontrolador ATmega328 da ATMEL e deve ser simulado no Proteus.
• O sistema deve utilizar no máximo dois temporizadores do ATmega328.
• Um botão de contato rápido será utilizado como incremento (I), outro como seletor (S) e um terceiro como debug (D).
• Quando o sistema inicia, após a tela de abertura, o sistema pergunta ao usuário o nível de dificuldade, apresentando a opção 4 (quatro) no display.
• O usuário deve, então, pressionar o botão de incremento para comutar ciclicamente o valor entre os níveis 4 (quatro) a 9 (nove) e selecionar o nível com o botão seletor.
• Após a seleção do nível de dificuldade, o sistema gera quatro números pseudo-aleatórios distintos, mas não os apresenta ao usuário; esta será a senha secreta.
• A senha secreta é composta por quadro dígitos distintos de 1 (um) ao nível de dificuldade selecionado.
• Neste momento, o sistema deixa o usuário digitar quatro números, digito por dígito, utilizando os botões (não deve ser apresentado um dígito fora do nível de dificuldade selecionado); esta é a senha do usuário.
• O sistema verifica se a senha do usuário é válida ou não; se não for válida, o sistema indica ao usuário através de quatro "X" no lugar da senha e um bip sonoro de um segundo; caso seja válida, o sistema a compara com a senha secreta.
• A comparação das senhas é feita dígito a dígito, e o resultado é apresentado no display no formato “n ->x.y”,
onde n é o número de tentativas, x é o número de dígitos da senha do usuário que estão na senha secreta porém
na posição errada, e y é o número dígitos da senha do usuário que estão na senha secreta na posição correta.
• Quando o usuário acertar a senha, dois bips tocam, uma mensagem de saudação é apresentada e o jogo reinicia
pedindo novamente o nível de dificuldade.
• A qualquer momento, o usuário pode pressionar o botão de debug; o botão de debug apresenta a senha secreta
ao usuário enquanto o botão estiver pressionado.
• Todos os botões devem ser dotados de anti-repique.
• O sistema só deve realizar a tarefa quando o botão for liberado.
• Um realce especial no código deve ser apresentado na parte de geração da senha secreta:
////////////////////////////////////////////////////////////////////////////////
// Realce de código – geração de senha secreta /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
...
...
...
////////////////////////////////////////////////////////////////////////////////
// Fim do Realce ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
