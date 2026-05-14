// --- Definições Globais --
#define ADC_MAX_VALUE           4095U   // Valor máximo para ADC de 12 bits
#define ADC_REFERENCE_VOLTAGE   3.3F    // Tensão de referência do ADC

#define FILTER_BUFFER_SIZE      16      // Número de amostras para a média móvel

#define SAMPLING_PERIOD_US      500U  // Período de amostragem (10 ms → 100 Hz)

#define GRAPH_BUFFER_SIZE  256U

extern float g_graphBuffer[2][GRAPH_BUFFER_SIZE];   // two buffers
extern unsigned int g_graphBufferIndex;
extern unsigned int g_graphActiveBuffer;             // which one firmware is writing
extern unsigned int g_graphBufferReady;              // signals CCS which one to read

// --- Enumeração para o Estado do Canal ADC ---
typedef enum {
    ADC_CHANNEL_STATE_DISABLED,
    ADC_CHANNEL_STATE_NORMAL,
    ADC_CHANNEL_STATE_OVER_VALUE
} AdcChannelState_t;

// --- Estrutura (Struct) para o Canal ADC ---
typedef struct {
    unsigned int      buffer[FILTER_BUFFER_SIZE]; // Buffer circular
    unsigned int      currentIndex;               // Índice atual no buffer
    unsigned int      filteredValueADC;           // Valor filtrado (contagens ADC)
    float             filteredVoltage;             // Valor filtrado em Volts
    AdcChannelState_t state;                      // Estado atual do canal
} AdcChannel_t;

// Protótipos de Funções
void initAdcChannel(AdcChannel_t *pCh);
void processAdcChannel(AdcChannel_t *pChannel);
unsigned int readSimulatedADC(void);
void addSampleToBuffer(AdcChannel_t *pChannel, unsigned int newSample);
void calculateMovingAverage(AdcChannel_t *pChannel);
float convertADCToVoltage(unsigned int adcValue);