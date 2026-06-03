/*
 * qbuffer.c
 *
 *  Created on: 2026. 2. 15.
 *      Author: yougang
 */
#include "qbuffer.h"

#ifdef _USE_QBUFFER
bool qbufferInit(void)
{
  bool ret=true;
  return ret;
}

bool qbufferCreate(qbuffer_t *p_node, uint8_t* p_buf, uint32_t length)
{
  bool ret=true;
  p_node->rx_in=0;
  p_node->rx_out=0;
  p_node->p_buf=p_buf;
  p_node->len=length;
  return ret;
}

bool qbufferWrite(qbuffer_t *p_node, uint8_t* p_data, uint32_t length)
{
  bool ret=true;
  uint32_t next_in;
  for(int i=0; i<length; i++)
  {
    next_in=(p_node->rx_in+1)%p_node->len;
    if(next_in != p_node->rx_out)
    {
      if(p_node->p_buf != NULL)
      {
         p_node->p_buf[p_node->rx_in]=p_data[i];
      }
      p_node->rx_in = next_in;
    }
    else
    {
      ret=false;
      return ret;
    }
  }
  return ret;
}

bool qbufferRead(qbuffer_t* p_node, uint8_t* p_data, uint32_t length)
{
  bool ret=true;
  for(int i=0; i<length; i++)
  {
    if(p_node->p_buf !=NULL)
    {
      p_data[i]=p_node->p_buf[p_node->rx_out];
    }
    if(p_node->rx_out != p_node->rx_in)
    {
      p_node->rx_out = (p_node->rx_out+1)%p_node->len;
    }
    else
    {
      ret=false;
      return ret;
    }
  }
  return ret;
}
uint32_t qbufferAvailable(qbuffer_t *p_node)
{
  uint32_t ret;
  ret=(p_node->len+p_node->rx_in-p_node->rx_out)%p_node->len;
  return ret;
}

bool qbufferflush(qbuffer_t *p_node)
{
  p_node->rx_in=0;
  p_node->rx_out=0;
  return true;
}
#endif
