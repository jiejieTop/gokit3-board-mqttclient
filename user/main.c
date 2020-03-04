#include "stm32f1xx_hal.h"
#include "bsp_init.h"
#include "tos_k.h"
#include "esp8266.h"
#include "mqttclient.h"

k_task_t task1;
k_stack_t task_stack1[1024];

mqtt_client_t client;
client_init_params_t init_params;

void test_task1(void *Parameter)
{
    int error;
    dht11_data_t dht11_data;
    char buf[80] = { 0 };
    mqtt_message_t msg;
    
    memset(&msg, 0, sizeof(msg));
    
    esp8266_sal_init(HAL_UART_PORT_2);
    esp8266_join_ap("wifii", "woshijiejie");
    
    init_params.read_buf_size = 128;
    init_params.write_buf_size = 128;
    init_params.connect_params.network_params.addr = "129.204.201.235";//"jiejie01.top"; //"47.95.164.112";//"jiejie01.top"; //"129.204.201.235"; //"192.168.1.101";
    init_params.connect_params.network_params.port = "1883";
    init_params.connect_params.user_name = random_string(10); // random_string(10); //"jiejietop-acer1";
    init_params.connect_params.password = random_string(10);; //random_string(10); // "123456";
    init_params.connect_params.client_id = random_string(10);; //random_string(10); // "clientid-acer1";
    init_params.connect_params.clean_session = 1;

    log_init();

    mqtt_init(&client, &init_params);

    error = mqtt_connect(&client);
    
    LOG_D("mqtt connect error is %#x", error);
    
    mqtt_subscribe(&client, "testtopic-gokit", QOS0, NULL);
    
    while(1)
    {
        tos_task_delay(2000); 
        
        tos_cpu_int_disable();

        error = dht11_read(&dht11_data);
        
        tos_cpu_int_enable();
        
        if(error == SUCCESS) {
            sprintf(buf, "temperature = %f, humidity = %f\n", dht11_data.temperature, dht11_data.humidity);
            
            msg.qos = QOS0;
            msg.payload = (void *) buf;
            
            mqtt_publish(&client, "testtopic-gokit", &msg);
            
            printf("mqtt publish to testtopic-gokit topic!\n");
        } else {
            printf("dht11 read error... \n");
        }
        
//        motor_set_speed(MOTOR_FORWARD, 20);
//        
//        tos_task_delay(2000); 
//        
//        motor_set_speed(MOTOR_FORWARD, 80);
    }
}


int main(void)
{

    k_err_t err;
    
    bsp_init();
    
    printf("Welcome to TencentOS tiny\r\n");

    tos_knl_init(); // TOS Tiny kernel initialize
    
    printf("create task1\r\n");
    err = tos_task_create(&task1, 
                          "task1", 
                          test_task1,
                          NULL, 
                          3, 
                          task_stack1,
                          1024,
                          20);
    if(err != K_ERR_NONE)
        printf("TencentOS Create task1 fail! code : %d \r\n",err);
    
    tos_knl_start(); // Start TOS Tiny

}
