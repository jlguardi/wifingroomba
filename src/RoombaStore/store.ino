/**
 * Compute CRC of input data pointer
 * @param data: pointer to data
 * @param size: size in bytes of data
 */
unsigned long data_crc(void* data, size_t size) {

  const unsigned long crc_table[16] = {
    0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
    0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
    0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
    0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
  };

  unsigned long crc = ~0L;
  unsigned char* _data = (unsigned char*)data;
  for (int index = 0 ; index < size  ; ++index) {
    crc = crc_table[(crc ^ _data[index]) & 0x0f] ^ (crc >> 4);
    crc = crc_table[(crc ^ (_data[index] >> 4)) & 0x0f] ^ (crc >> 4);
    crc = ~crc;
  }
  return crc;
}

/**
 * Convenience function to compute crc from structure or basic type:
 * @param _A: struct/basic type variable
 * @example:
 *     my_struct_t d;
 *     unsigned long crc_value = crc(d);
 */
#define crc(_A) data_crc((void*)&(_A),sizeof(_A))

/** EEPROM size **/
const int memsize = 512;

#define ct_assert(e) ((void)sizeof(char[1 - 2*!(e)])) // If you get a compilation error here means that assert failed

/** Load config from EEPROM. config should be of any const size type without pointers.
 *  default_config_values is used if crc fails and should have the same type as config.
 *  i.e:
 *  typedef struct{
 *    int a;
 *    float b;
 *    char c;
 *    char str[128];
 *  }config_t;
 *  config_t config;
 *  
 *  @example:
 *    typedef struct{int a;}config_t;
 *    const config_t default_config_values = {100};
 *    config_t config;
 *    bool ret = loadStore();
 *  @return true on success or false if error (and defaults are set)
 */
bool loadStore() {
  
  ct_assert((sizeof(crc(config))+sizeof(config))<memsize);

  EEPROM.begin(memsize);
  EEPROM.get(0, config);
  unsigned long read_crc;
  EEPROM.get(0+sizeof(config), read_crc);
  EEPROM.end();

  if (read_crc != crc(config)) { // load defaults
    Serial.println("CRC doesn't match. Setting defaults...");
    memcpy(&config, &default_config_values, sizeof(config));
    return false;
  }
  return true;
}

/** Store config to EEPROM. config should be of any const size type without pointers.
 *  
 */
void saveStore() {
  
  ct_assert((sizeof(crc(config))+sizeof(config))<memsize);
  
  EEPROM.begin(memsize);
  EEPROM.put(0, config);
  EEPROM.put(0+sizeof(config), crc(config));
  EEPROM.commit();
  EEPROM.end();
}
