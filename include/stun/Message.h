/*

  Message
  --------

  Represents a stun::Message. When you're using Message-Integrity and Fingerprint
  attributes, make sure to call computeMessageIntegrity(), before you call computeFingerprint().
  The fingerprint (crc32) is computer over the buffer including the message-integrity value, whne 
  you don't compute this first, the crc will be incorrect.

 */
#ifndef STUN_MESSAGE_H
#define STUN_MESSAGE_H

#include <stun/Attribute.h>
#include <stun/Types.h>

namespace stun {

  class Message {
  public:
    Message(uint16_t type = STUN_MSG_TYPE_NONE);
    ~Message();
    void addAttribute(Attribute* attr);                        /* Add an attribute to the message who takes ownership (will delete all attributes in the d'tor. */
    void copyTransactionID(Message* from);                     /* Copy the transaction ID from the given messsage. */
    void setTransactionID(uint32_t a, uint32_t b, uint32_t c); /* Set the transaction ID from the given values. */
    bool hasAttribute(AttributeType atype);                    /* Check if the given attribute is found in one of the attributes */
    bool find(MessageIntegrity** result);                      /* Find a message integrity attribute. */
    bool find(XorMappedAddress** result);                      /* Find a xor-mapped-address attribute.*/
    bool find(Fingerprint** result);                           /* Find a fingerprint attrbiute. */
    bool computeMessageIntegrity(std::string key);             /* When the message contains a MessageIntegrity element, this will compute the HMAC-SHA1 message integrity. */
    bool computeFingerprint();                                 /* When the message contains a Fingerprint attriute (must be added after the MessageInterity attribute), this will calculate and set CRC value. Important: make sure that you computer the fingerprint AFTER you've computed the message-integrity  */

    template<class T> bool find(uint16_t atype, T** result) {
      *result = NULL;
      for (size_t i = 0; i < attributes.size(); ++i) {
        if (attributes[i]->type == atype) {
          *result = static_cast<T*>(attributes[i]);
          return true;
        }
      }
      return false;
    }

  public:
    uint16_t type;
    uint16_t length;
    uint32_t cookie;
    uint32_t transaction[3];
    std::vector<Attribute*> attributes;
    std::vector<uint8_t> buffer;
  };

} /* namespace stun */

#endif
