//#############################################################################
//
// ARQUIVO:    pwm_soft.c
//
// TÍTULO:    Geração de PWM por Software e Configuração Simulada
//
//! Este exemplo gera um PWM por software e simula sua configuração via registrador.
//! O registrador contém apenas o valor de comparação (duty‑cycle) e um bit de enable.
//! Observar o brilho do LED e variáveis no depurador do CCS.
//
//#############################################################################
//
// $Data de Lançamento: $
// $Copyright:
// Copyright (C) 2013-2024 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribuição e uso em formatos de código-fonte e binários, com ou sem
// modificação, são permitidos desde que as seguintes condições sejam
// atendidas:
//
//   As redistribuições do código-fonte devem reter o aviso de direitos autorais
//   acima, esta lista de condições e a seguinte isenção de responsabilidade.
//
//   As redistribuições em formato binário devem reproduzir o aviso de direitos autorais
//   acima, esta lista de condições e a seguinte isenção de responsabilidade na
//   documentação e/ou outros materiais fornecidos com a distribuição.
//
//   Nem o nome da Texas Instruments Incorporated nem os nomes de
//   seus colaboradores podem ser usados para endossar ou promover produtos derivados
//   deste software sem permissão prévia por escrito.
//
// ESTE SOFTWARE É FORNECIDO PELOS DETENTORES DOS DIREITOS AUTORAIS E COLABORADORES
// "AS IS" E QUAISQUER GARANTIAS EXPRESSAS OU IMPLÍCITAS, INCLUINDO, MAS NÃO
// SE LIMITANDO A, AS GARANTIAS IMPLÍCITAS DE COMERCIALIZAÇÃO E ADEQUAÇÃO PARA
// UM PROPÓSITO ESPECÍFICO SÃO REJEITADAS. EM NENHUM CASO O DETENTOR DOS DIREITOS AUTORAIS
// OU COLABORADORES SERÃO RESPONSÁVEIS POR QUAISQUER DANOS DIRETOS, INDIRETOS, INCIDENTAIS,
// ESPECIAIS, EXEMPLARES OU CONSEQUENCIAIS (INCLUINDO, MAS NÃO SE LIMITANDO A,
// AQUISIÇÃO DE BENS OU SERVIÇOS SUBSTITUTOS; PERDA DE USO, DADOS OU LUCROS;
// OU INTERRUPÇÃO DE NEGÓCIOS) SEJA QUAL FOR A CAUSA E SOB QUALQUER TEORIA DE
// RESPONSABILIDADE, SEJA EM CONTRATO, RESPONSABILIDADE ESTRITA OU ATO ILÍCITO
// (INCLUINDO NEGLIGÊNCIA OU OUTRO) DECORRENTE DE QUALQUER FORMA DO USO DESTE
// SOFTWARE, MESMO SE AVISADO DA POSSIBILIDADE DE TAL DANO.
// $
//#############################################################################

// Arquivos Incluídos
#include "driverlib.h"
#include "device.h"
#include <stdbool.h> // Para tipo bool
#include "adc_simulator/adc_simulator.h"
#include "pwm_simulator/pwm_simulator.h"


AdcChannel_t g_adcChannel;

// Função Principal
void main(void)
{
    initSystemPeripherals();
    initLEDGPIO();

    // Configura o ciclo de trabalho inicial e o registrador simulado
    setPWMDutyCycleAndRegister(g_dutyCyclePercent);
    initAdcChannel(&g_adcChannel);

    // Loop infinito para gerar o PWM
    for(;;)
    {
            if (g_enable_pwm)
    {
        enablePWM();
        
        processAdcChannel(&g_adcChannel);

        // Atraso para simular período de amostragem
        DEVICE_DELAY_US(SAMPLING_PERIOD_US);
    }

    // else {
    // {
    //     disablePWM();
    // }
    // }
        // setPWMDutyCycleAndRegister(g_dutyCyclePercent);
        // generateSoftwarePWM();
        // g_dutyCyclePercent++;
       // DEVICE_DELAY_US(10000);  // 10 ms
        // if (g_dutyCyclePercent > 100.0f)
        // {
        //     g_dutyCyclePercent = 0.0f;
        //     DEVICE_DELAY_US(500000);  // 500 ms
        // }
        
    }
}


