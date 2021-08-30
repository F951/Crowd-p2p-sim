#ifndef _NODE_ENTRY_H
#define _NODE_ENTRY_H
#include "../glob.h"

// Node Entry class encapsulates nodes information
// as nodeID and Ip address
class NodeEntry
{

public:

    int ip;
    BIGNUM *nodeID;

    ~NodeEntry()
    {
        //      free(nodeID);
    }

    // Constructor
    NodeEntry()
    {
	}
    NodeEntry(BIGNUM *_nodeID, int  _ip)
    {
        //  cout << "new node entry" << endl;
        ip = _ip;
        nodeID = _nodeID;
    }

    // Returns node Ip address
    // Returns: int node index
    int getIP()
    {
        return ip;
    }

    // Returns Pastry NodeID
    // Returns: BIGNUM nodeID
    BIGNUM *getNodeID()
    {
        return nodeID;
    }
    
    // Returns node Ip address
    // Returns: int node index
    void setIP( int _ip)
    {
        ip = _ip;
    }

    // Returns Pastry NodeID
    // Returns: BIGNUM nodeID
    void setNodeID(BIGNUM* _nodeID)
    {
        nodeID = _nodeID;
    }

};
#endif
