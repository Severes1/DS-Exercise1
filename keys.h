/*
 * This call allows the initialization of the system. Through this service,
 * all triples that were stored are destroyed.
 * The function returns 0 on success and -1 on error.
 * */
int init(void);

/*
 * This service inserts the element <key, value1, value2>. 
 * The service returns 0 if it was inserted successfully and -1 in case of error.
 * It is considered an error to try to insert a key that already exists.
 * */
int set_value(int key, char * value1, float value2);

/*
 * This service allows you to obtain the values associated with the key. 
 * The values are returned in value1 and value2.
 * The function returns 0 on success and -1 on error, for example,
 * if there is no element with that key.
*/
int get_value(int key, char * value1, float * value2);

/*
 * This service allows modifying the values associated with a key. 
 * The function returns 0 on success and -1 on error, for example, 
 * if there is no element with that key.
*/ 
int modify_value(int key, char * value1, float * value2);

/*
 * This service allows you to delete the element whose key is key. 
 * The function returns 0 on success and -1 on error. In case the key 
 * does not exist, -1 is also returned.
*/
int delete_key(int key);

/*
 * This service returns the number of items stored on the server. 
 * The call returns -1 in case of error
*/
int num_items();
