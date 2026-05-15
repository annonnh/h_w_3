#include "adc_simulator.h"
#include "input_signal/input_signal.h"
#include "pwm_simulator/pwm_simulator.h"

#define ADC_DC_OFFSET_VOLTAGE  ((2048.0F / (float)ADC_MAX_VALUE) * ADC_REFERENCE_VOLTAGE)

float limiar_max_value = 0.0F;
int sample_state = 0U;
float sin_function_with_noise = 0.0F;
float sin_function_0_centered = 0.0F;

// At the top, alongside your other globals like sin_function_with_noise
float g_graphBuffer[2][GRAPH_BUFFER_SIZE];
unsigned int g_graphBufferIndex    = 0U;
unsigned int g_graphActiveBuffer   = 0U;
unsigned int g_graphBufferReady    = 0U;             // watch this in CCS

// Implementações de Funções

void initAdcChannel(AdcChannel_t *pCh)
{
    unsigned int i;
    // Limpa o buffer do filtro
    for (i = 0U; i < FILTER_BUFFER_SIZE; i++)
    {
        pCh->buffer[i] = 0U;
    }
    pCh->currentIndex = 0U;
    pCh->filteredValueADC = 0U;
    pCh->filteredVoltage = 0.0F;
    pCh->state = ADC_CHANNEL_STATE_NORMAL;
}

// Processa um ciclo completo: leitura, filtro e verificação de limiar
void processAdcChannel(AdcChannel_t *pChannel)
{
    // 1. Lê o ADC (simulado)
    unsigned int rawSample = read_simulated_ADC_with_sin();

    // 2. Adiciona a nova amostra ao buffer circular
    addSampleToBuffer(pChannel, rawSample);

    // 3. Calcula a média móvel
    calculateMovingAverage(pChannel);

    // 4. Converte o valor filtrado para tensão
    pChannel->filteredVoltage = convertADCToVoltage(pChannel->filteredValueADC);
    sin_function_with_noise = convertADCToVoltage(rawSample);
    //sin_function_with_noise = pChannel->filteredVoltage;
    
    sin_function_0_centered = pChannel->filteredVoltage - ADC_DC_OFFSET_VOLTAGE;
    
    g_dutyCyclePercent = fabs((double)(sin_function_0_centered*200.0F));

    if (sin_function_0_centered >= 0)
    {
        select_led = 1; // green led
    }

    else
    {
        select_led = 0; // blue led
    }
    // Fill the active buffer
    g_graphBuffer[g_graphActiveBuffer][g_graphBufferIndex] = sin_function_0_centered;//sin_function_with_noise;
    g_graphBufferIndex++;

    if (g_graphBufferIndex >= GRAPH_BUFFER_SIZE)
    {
        g_graphBufferIndex = 0U;

        // Swap buffers — firmware moves to the other one
        g_graphActiveBuffer = 1U - g_graphActiveBuffer;

        // Tell CCS which buffer is now complete and safe to read
        g_graphBufferReady  = 1U - g_graphActiveBuffer;
    }
        //5. Aqui deverá ser incluída a lógica de detecção de limiar excedido
        if (pChannel->filteredVoltage >= limiar_max_value)
        {
            pChannel->state = ADC_CHANNEL_STATE_OVER_VALUE;
        }
        
        else
        {
            pChannel->state = ADC_CHANNEL_STATE_NORMAL;
        }
        sample_state = pChannel->state; 
}

// Simula uma leitura do ADC gerando um valor que varia lentamente.
// Utiliza um contador simples para produzir uma rampa triangular
// (sobe de 0 até ADC_MAX_VALUE e depois desce).
unsigned int readSimulatedADC(void)
{
    static unsigned long counter = 0UL;
    static int direction = 1;           // 1 = subindo, -1 = descendo

    // Atualiza o contador com passo fixo
    counter = counter + (unsigned long)(10 * direction);

    // Inverte a direção nos extremos
    if (counter >= ADC_MAX_VALUE)
    {
        counter = ADC_MAX_VALUE;
        direction = -1;
    }
    else if (counter == 0UL)
    {
        direction = 1;
    }

    return (unsigned int)counter;
}

// Adiciona uma nova amostra ao buffer circular do canal.
void addSampleToBuffer(AdcChannel_t *pChannel, unsigned int newSample)
{
    pChannel->buffer[pChannel->currentIndex] = newSample;
    pChannel->currentIndex = (pChannel->currentIndex + 1U) % FILTER_BUFFER_SIZE;
}

// Calcula a média móvel das amostras no buffer.
void calculateMovingAverage(AdcChannel_t *pChannel)
{
    unsigned long sum = 0UL;
    unsigned int i;
    for ( i = 0U; i < FILTER_BUFFER_SIZE; i++)
    {
        sum = sum + pChannel->buffer[i];
    }
    // Arredondamento simples
    pChannel->filteredValueADC = (unsigned int)((sum + FILTER_BUFFER_SIZE / 2U) / FILTER_BUFFER_SIZE);

    // Garante que o valor não ultrapasse o máximo do ADC
    if (pChannel->filteredValueADC > ADC_MAX_VALUE)
    {
        pChannel->filteredValueADC = ADC_MAX_VALUE;
    }
}

// Converte um valor ADC (0 a 4095) para tensão (0 a 3.3 V).
float convertADCToVoltage(unsigned int adcValue)
{
    return ((float)adcValue / (float)ADC_MAX_VALUE) * ADC_REFERENCE_VOLTAGE;
}
