# Identificador de cor
O sensor TCS3200 que mede a intensidade luminosa em 3 faixas de cores: Vermelho, Verde e Azul. O sensor é composto por uma matriz de receptores igualmente distribuídos na superfície do circuito integrado. A cor da luz que queremos realizar a leitura através dos bits S2 e S3, por exemplo vermelha (S2 = 0 e S3 = 0). A luz detectada é convertida num sinal periódico cuja frequência é proporcional à intensidade luminosa. 

Fazemos então a leitura da frequência do sinal de saída. Podemos também alterar a faixa da frequência gerada pelos bits S0 e S1. A frequência em 100% é de 16kHz aproximadamente.  S2 S3 Filtro S0 S1 Escala da freq. de saída 0 0 Vermelho 0 0 0% 0 1 Azul 0 1 2% 1 0 Sem filtro 1 0 20% 1 1 Verde 1 1 100%. 

Foi desenvolvido um programa que consegue diferenciar as cores vermelho, verde e azul mostrando nos LEDs da launchpad as cores percebidas pelo sensor, acendendo o LED vermelho quando o vermelho for predominante, o LED verde quando a cor verde for predominante e acenda ambos os LEDs quando o azul for predominante.

