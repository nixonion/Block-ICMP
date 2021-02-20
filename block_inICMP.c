/* 
	Nabeel Mahdi Abid Mehdi Sayed
	G01213257
*/

#define DEBUG

#include "divertlib.h"

#define maxbuflen 1520

int main(int argc, char *argv[])
{ int i, len, divsock;
  u_short iphlen, tcphlen;
  int udpsock, DivPort;
  struct sockaddr_in sin, sin1;
  struct ip *iph;
  struct tcphdr *tcph;
  struct udphdr *udph;
  unsigned char buf[maxbuflen+1];
  int addrsize=sizeof (struct sockaddr);
  time_t start,end;

  if (argc!=2)
  { puts("usage : divert-loop [div port]");
    return 1;
  };

  DivPort=atoi(argv[1]);
  printf("DivPort=%d\n", DivPort);

  if ((divsock=initDivSock(DivPort))<=0)
  { printf("can not get divert socket for port %d, divsock=%d\n",
            DivPort, divsock);
    exit(1);
  };

  for (i=1; ;i++)
  {
/* if ((len=readDiv(divsock, buf, maxbuflen, (struct sockaddr *) &sin))>0)
 */
    if ((len=recvfrom(divsock, buf, maxbuflen, 0, 
		      (struct sockaddr *) &sin, &addrsize))>0)
    {

      iph=(struct ip *) buf;
      iphlen=iph->ip_hl<<2;
#ifdef DEBUG
	
      if (sin.sin_addr.s_addr==INADDR_ANY) /* outgoing */
      { printf("\n%d : Out\t\t\t\t\t\t\t\t==>\n", i);
	printf("\tsrc IP:%s\n", inet_ntoa(iph->ip_src));
      	printf("\tdst IP:%s\n", inet_ntoa(iph->ip_dst));
	//printf("\tType:%02x\n", buf[iphlen]);
	//printf("\tTTL :%d\n", iph->ip_ttl);
      	printf("\tproto :%d\n", iph->ip_p);
	start=time(NULL);
	sendto(divsock, buf, len, 0, (struct sockaddr *) &sin,
	     sizeof(struct sockaddr));

	
      }
      else /* incoming */
      { 	
	if((iph->ip_p)==1 && buf[iphlen]==0x00)
	{
		printf("\n%d : In from %s:%d\t\t\t\t\t\t<==\n",
                i, inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));
		printf("\tsrc IP:%s\n", inet_ntoa(iph->ip_src));
      		printf("\tdst IP:%s\n", inet_ntoa(iph->ip_dst));
		//printf("\tType:%02x\n", buf[iphlen]);
      		printf("\tproto :%d\n", iph->ip_p);
		//printf("\tTTL :%d\n", iph->ip_ttl);
		//printf("\tTTL :%02x\n", buf[iphlen+2]);
		end=time(NULL);
		//printf("%ld n",end);
		if((end-start) <= 60)
		{
		//printf("%ld",(end-start));
		sendto(divsock, buf, len, 0, (struct sockaddr *) &sin,
	     sizeof(struct sockaddr));
		}
	

	}
	else
	{
		
		printf("\n%d : Reject from %s:%d\t\t\t\t\t\t<==\n",i,inet_ntoa(sin.sin_addr),ntohs(sin.sin_port));
	}
      };
      
	
#endif
      
    }
  };
  return 0;
}
 
