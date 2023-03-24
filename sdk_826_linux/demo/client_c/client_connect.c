#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
// #include <json-c/json.h>

#include <time.h>

#include <stdio.h>
#include <stdlib.h>


// #include "hashmap.h"
// #include "dynamicarray.h"
// #include "strstream.h"

// #include "json.h"
// #include "json/json.h"
#include <json-c/json.h>

#include "send_data.h"

#define PORT 4455

// struct json_object *parsed_json;

float function_exicution_time, position_x, F_ref, rtob_force,force_pred;

// struct dataset {        
//     float position, force;
//     int cycle_no;
//     char  title[50];
// };

// struct problem {        
//     char letter[1];
//     int number[1];
// };
void back_to_main(){
    printf("\n Back to main function \n");
}

float json_parse(json_object * jobj) {
  enum json_type type;
  float force_pred =0.0;
  float x =0.0;
  json_object_object_foreach(jobj, key, val) {
    printf(key);
    
    
    // printf(&key);
    // if (key == "force"){
    //   printf("value_force: %lf \n", json_object_get_double(val));
    // }else{
    //   printf("k");
    // }
    type = json_object_get_type(val);
    switch (type) {
      case json_type_double: printf(" type: json_type_double, ");
                          printf("value: %lf \n", json_object_get_double(val));
                          //force_pred = (float) *val;
                          x = (double) json_object_get_double(val);
                          printf("x %lf\n",x);                         
                        
                          break;
    }
  }
  
  return x;
}
float get_val(float function_exicution_time,float position_x,float F_ref,float rtob_force){

  // force =4.09f;
  // cycle_no =1;

  // json OUT = json_type(JSON_OBJ);
  // bvf
    
  // json_obj_put_float(&OUT, "function_exicution_time", function_exicution_time);
  // json_obj_put_float(&OUT, "position_x", position_x);
  // json_obj_put_float(&OUT, "F_ref", F_ref);
  // json_obj_put_float(&OUT, "rtob_force", rtob_force);


  /*Creating a json object*/
  json_object * jobj = json_object_new_object();

  // /*Creating a json integer*/
  // json_object *jint = json_object_new_int(10);


  /*Creating a json double*/
  json_object *function_exicution_time_obj = json_object_new_double((double)function_exicution_time);
  json_object *position_x_obj = json_object_new_double((double)position_x);
  json_object *F_ref_obj = json_object_new_double((double)F_ref);
  json_object *rtob_force_obj = json_object_new_double((double)rtob_force);


  /*Form the json object*/
  /*Each of these is like a key value pair*/
  json_object_object_add(jobj,"function_exicution_time", function_exicution_time_obj);
  json_object_object_add(jobj,"position_x", position_x_obj);
  json_object_object_add(jobj,"F_ref", F_ref_obj);
  json_object_object_add(jobj,"rtob_force", rtob_force_obj);

  /*Now printing the json object*/
  printf ("json object: %s\n",json_object_to_json_string(jobj));


 // json_obj_put_int(&OUT, "cycle no.", cycle_no);

  // /*Creating a json object*/
  // json_object * jobj = json_object_new_object();
  // float force =4.09;
  // int cycle_no =1;
  // float marks[2]={20.09,67.89};  

  // /*Creating a json integer*/
  // json_object *jint = json_object_new_int(10);

  // /*Creating a json double*/
  // json_object *jdouble = json_object_new_double(force);

  
  // /*Form the json object*/
  // json_object_object_add(jobj,"Force", jdouble);
  // json_object_object_add(jobj,"Number of posts", jint);
  


  // dict m = {"id": 2, "f": 5.32, "name": "abc"} 
  // float array[]={3.4,5.3};
  //char *ip = "127.0.0.1";
  // int port = 4455;
  // struct dataset dataset1 ={2.36,5.23,1,"nn"};     

  int sock;
  struct sockaddr_in addr;
  socklen_t addr_size;
  char buffer[1024];
  int n;

  time_t seconds;

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0){
    perror("[-]Socket error");
    exit(1);
  }
  printf("[+]TCP server socket created.\n");

  memset(&addr, '\0', sizeof(addr));
  addr.sin_family = AF_INET;


  // Important to keep hton(Port)
  // addr.sin_port = port;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  connect(sock, (struct sockaddr*)&addr, sizeof(addr));
  printf("Connected to the server.\n");

  // memset(buffer, '\0', sizeof(buffer));
  // strcpy( Book1.title, "C Programming");

  // strcpy( dataset1.title, "Nuha");
  // send(sock, array, strlen(array), 0);



  // int a;
  // char *str = json_dump(jobj, true, &a);

  strcpy(buffer, json_object_to_json_string(jobj));
  // strcpy(buffer, "Hello, This is a test message");
  send(sock, buffer, strlen(buffer), 0);
  printf("%s\n", buffer);

  //write(sock, jobj, sizeof(jobj));
 

  
  
  // printf("%s\n", str);
  // free(str);

  // printf(dataset1.title);
   // Stores time seconds
  time(&seconds);
  printf("Time to send from client %ld\n", seconds);

  memset(buffer, '\0', sizeof(buffer));
  recv(sock, buffer, 1024, 0);
  printf("[CLIENT] %s\n", buffer);

  json_object * parsed_json = json_tokener_parse(buffer);

 
  

  printf ("json object _ received: %s \n",json_object_to_json_string(parsed_json));
  force_pred =json_parse(parsed_json);
  printf("force_pred- client connect  %f\n",force_pred);
	// json_object_object_get_ex(parsed_json, "force", &force);

	// printf("force: %.6f\n", json_object_get_int(force));



  // Stores time seconds
  time(&seconds);
  printf("Time to receive to client %ld.%.7ld\n", seconds);
  // printf("Time to receive to client %ld.%ld\n", seconds);
  // back_to_main();

  /* 
  bzero(buffer, 1024);
  strcpy(buffer, "HELLO, THIS IS CLIENT.");
  printf("Client: %s\n", buffer);
  send(sock, buffer, strlen(buffer), 0);
  // Stores time seconds
  time(&seconds);
  printf("Time to send to server %0.7ld\n", seconds);

  bzero(buffer, 1024);
  recv(sock, buffer, sizeof(buffer), 0);
  printf("Server: %s\n", buffer);
  time(&seconds);
  printf("Time to receive to server %ld.%.7ld\n", seconds);

  */

  close(sock);
  printf("Disconnected from the server.\n");

  return force_pred;

}
