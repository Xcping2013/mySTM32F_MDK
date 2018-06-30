/*
    @file     MCP342X.h
    @author   xcping2013

 *  ver1.0	  20171219	original
 *  ver1.1						  modify:
 */



#ifndef _MCP342X_H_
#define _MCP342X_H_

#ifdef __cplusplus
extern "C" {
#endif
	



class MCP342X {
    public:
        MCP342X();
        MCP342X(uint8_t address);
        
        bool testConnection(void);

        // Set/Get the configuration bits for the ADC
        void configure(uint8_t config);
        uint8_t getConfigRegShdw(void);
	   //float getStepSize(); // returns step size based on configRegShdw

        // Start a conversion
        bool startConversion(void);
        bool startConversion(uint8_t channel);
        
        // Read the ADC result
        uint8_t getResult(int16_t *data);
        uint8_t getResult(int32_t *data);

        // Non-blocking Read the ADC result
        uint8_t checkforResult(int16_t *data);
        uint8_t checkforResult(int32_t *data);

    private:
        uint8_t devAddr;
        uint8_t configRegShdw;
	//float	stepSizeTbl[];
};

#endif /* _MCP342X_H_ */
