#include <iostream>
#include <vector>
#include "Includes.h"
#include "Constants.h"
#include "circle.h"


//Nodes: goes in order of parent->left, left2, right, right 2
class Node
{
public:
    Node(int k, int minX, int maxX, int minY, int maxY,
         Node * p = NULL, Node * l = NULL, Node * l2 = NULL,
         Node * r = NULL, Node * r2 = NULL)
        : key(k), minx(minX), maxx(maxX), miny(minY), maxy(maxY),
          parent(p), left1(l), left2(l2), right1(r), right2(r2)
    {}

    void setLeft1(Node * l1)
    {
        this->left1 = l1;
    }

    void setLeft2(Node * l2)
    {
        this->left2 = l2;
    }
    
    void setRight1(Node * r1)
    {
        this->right1 = r1;
    }

    void setRight2(Node * r2)
    {
        this->right2 = r2;
    }

    Node * getLeft1()
    {
        return this->left1;
    }

    Node * getLeft2()
    {
        return this->left2;
    }
    
    Node * getRight1()
    {
        return this->right1;
    }

    Node * getRight2()
    {
        return this->right2;
    }

    Node * getParent()
    {
        return this->parent;
    }
    
    std::vector < Circle* > & getCircles()
    {
        return this->circles;
    }

    std::vector < Circle* > & getCirclesOnBorder()
    {
        return this->circlesOnBorder;
    }
    
    int size()
    {
        return circles.size(); 
    }

    //special case for first node. Makes sure all circles stored
    void storeAllCircles(const std::vector < Circle* > & c)
    {
        for (int i = 0; i < CIRCLES; ++i)
        {
            this->circles.push_back(c[i]);
        }
    }    

    //store only circles WITHIN boundaries
    void storeCircles(const std::vector < Circle* > & c)
    {
        for (int i = 0; i < c.size(); ++i)
        {
            if (c[i]->gx() > this->minx && c[i]->gy() > this->miny
                && c[i]->gx() < this->maxx && c[i]->gy() < this->maxy)
            {
                this->circles.push_back(c[i]);
            }
            else if (c[i]->gx() == this->minx || c[i]->gy() == this->miny
                     || c[i]->gx() == this->maxx || c[i]->gy() == this->maxy)
            {
                this->circlesOnBorder.push_back(c[i]);
            }
        }
        circles.shrink_to_fit();
    }

    int minX() { return minx; }
    int minY() { return miny; }
    int maxX() { return maxx; }
    int maxY() { return maxy; }
    
private:
    int key;
    int minx;
    int maxx;
    int miny;
    int maxy;
    Node * parent;
    Node * left1;
    Node * left2;
    Node * right1;
    Node * right2;

    std::vector< Circle* > circles;
    std::vector< Circle* > circlesOnBorder;
};


class Tree
{
public:
    Tree(const std::vector < Circle* > & c)
        : numNodes(1)
    {
        n0 = new Node(0, 0, W, 0, H);
        n0->storeAllCircles(c);
        int depth = -1;
        buildTree(n0, 0, W, 0, H, n0->getCircles(), depth);
        //std::cout << numNodes << '\n';
    }

    ~Tree()
    {
        Node * p = n0;
        deleteTree(p);
    }

    bool deleteTree(Node * p)
    {
        if (p->getLeft1() == NULL && p->getLeft2() == NULL &&
            p->getRight1() == NULL && p->getRight2() == NULL)
        {
            delete p;
            return 1;
        }
        else
        {
            if (p->getLeft1() != NULL) deleteTree(p->getLeft1());
            if (p->getLeft2() != NULL) deleteTree(p->getLeft2());
            if (p->getRight1() != NULL) deleteTree(p->getRight1());
            if (p->getRight2() != NULL) deleteTree(p->getRight2());
        }
    }

    int nodes() const
    {
        return numNodes;
    }

    Node * getTop()
    {
        return n0;
    }
    
    bool drawNodes(Surface & surface, Node * p)
    {
        if (p == NULL)
        {
            return 1;
        }
        else
        {
            drawNodes(surface, p->getLeft1());
            drawNodes(surface, p->getLeft2());
            drawNodes(surface, p->getRight1());
            drawNodes(surface, p->getRight2());
        }   
         
        if (p->getLeft1() != NULL)
        {
            Node * n1 = p->getLeft1();
            surface.put_line(n1->minX(), n1->maxY(), n1->maxX(), n1->maxY(), GREEN);
            surface.put_line(n1->maxX(), n1->minY(), n1->maxX(), n1->maxY(), GREEN);
        }

        if (p->getLeft2() != NULL)
        {
            Node * n2 = p->getLeft2(); 
            surface.put_line(n2->minX(), n2->minY(), n2->minX(), n2->maxY(), GREEN);
            surface.put_line(n2->minX(), n2->maxY(), n2->maxX(), n2->maxY(), GREEN);
        }

        if (p->getRight1() != NULL)
        {
            Node * n3 = p->getRight1();
            surface.put_line(n3->minX(), n3->minY(), n3->maxX(), n3->minY(), GREEN);
            surface.put_line(n3->maxX(), n3->minY(), n3->maxX(), n3->maxY() - 1, GREEN);
        }

        if (p->getRight2() != NULL)
        {
            Node * n4 = p->getRight2();
            surface.put_line(n4->minX(), n4->minY(), n4->maxX() - 1, n4->minY(), GREEN);
            surface.put_line(n4->minX(), n4->minY(), n4->minX(), n4->maxY() - 1, GREEN);
        }
    }

    int max(int x, int y)
    {
        return (x >= y ? x : y);
    }
    int depth(Node * n)
    {
        if (n == NULL)
        {
            return -1;
        }
        else
        {
            return 1 + max(max(depth(n->getLeft1()), depth(n->getLeft2())),
                           max(depth(n->getRight1()), depth(n->getRight2())));
        }
    }

    
    //make a recursive set of 4 nodes per branch
    bool buildTree(Node * parent, int minX, int maxX, int minY, int maxY,
                   std::vector < Circle* > & c, int depth)
    {
        ++depth;
        //parent 1eft
        Node * l1 = new Node(numNodes, minX, ((maxX / 2) + (minX / 2)), minY,
                             ((maxY / 2) + (minY / 2)), parent);
        ++numNodes;
        
        //parent left2
        Node * l2 = new Node(numNodes, ((maxX / 2) + (minX / 2)), maxX, minY,
                             ((maxY / 2) + (minY / 2)), parent);
        ++numNodes;
        
        //parent right
        Node * r1 = new Node(numNodes, minX, ((maxX / 2) + (minX / 2)),
                             ((maxY / 2) + (minY / 2)), maxY, parent);
        ++numNodes;
        
        //parent right2
        Node * r2 = new Node(numNodes, ((maxX / 2) + (minX / 2)), maxX,
                             ((maxY / 2) + (minY / 2)), maxY, parent);
        ++numNodes;

        // std::cout << "depth: " << depth << '\n';
        // std::cout << "l1: " << l1 << " minX: " << l1->minX()
        //           << " minY: " << l1->minY()
        //           << " maxX: " << l1->maxX() 
        //           << " maxY: " << l1->maxY() << '\n';
        // std::cout << "l2: " << l2 << " minX: " << l2->minX()
        //           << " minY: " << l2->minY()
        //           << " maxX: " << l2->maxX() 
        //           << " maxY: " << l2->maxY() << '\n';
        // std::cout << "r1: " << r1 << " minX: " << r1->minX()
        //           << " minY: " << r1->minY()
        //           << " maxX: " << r1->maxX() 
        //           << " maxY: " << r1->maxY() << '\n';
        // std::cout << "r2: " << r2 << " minX: " << r2->minX()
        //           << " minY: " << r2->minY()
        //           << " maxX: " << r2->maxX() 
        //           << " maxY: " << r2->maxY() << "\n\n";
        
        parent->setLeft1(l1);
        parent->setLeft2(l2);
        parent->setRight1(r1);
        parent->setRight2(r2);

        l1->storeCircles((parent->getCircles()));
        l2->storeCircles((parent->getCircles()));
        r1->storeCircles((parent->getCircles()));
        r2->storeCircles((parent->getCircles()));

        if (parent->getCirclesOnBorder().size() > 0)
        {
            nodeCollisionBorder(parent->getCircles(),
                                parent->getCirclesOnBorder());
        }
        
        if (l1->size() > THRESH && depth < MAXDEPTH)
        {  
            buildTree(l1, l1->minX(), l1->maxX(), l1->minY(), l1->maxY(),
                      l1->getCircles(), depth);
        }
        else
        {
            nodeCollision(l1->getCircles());
        }

        if (l2->size() > THRESH && depth < MAXDEPTH)
        {
            buildTree(l2, l2->minX(), l2->maxX(), l2->minY(), l2->maxY(),
                      l2->getCircles(), depth);
        }
        else
        {
            nodeCollision(l2->getCircles());
        }
        
        if (r1->size() > THRESH && depth < MAXDEPTH)
        {
            buildTree(r1, r1->minX(), r1->maxX(), r1->minY(), r1->maxY(),
                      r1->getCircles(), depth);
        }
        else
        {
            nodeCollision(r1->getCircles());
        }

        if (r2->size() > THRESH && depth < MAXDEPTH)
        {
            buildTree(r2, r2->minX(), r2->maxX(), r2->minY(), r2->maxY(),
                      r2->getCircles(), depth);
        }
        else
        {
            nodeCollision(r2->getCircles());
        }
        
//if > threshold, make more nodes.  else return
    }

    bool nodeCollision(std::vector< Circle*> & c)
    {
        for (int i = 0; i < c.size(); ++i)
        {
            for (int j = i + 1; j < c.size(); ++j)
            {
                c[i]->checkCollision(*c[j]);
            }
        }
    }

    //checks border circles against all circles in parent
    bool nodeCollisionBorder(std::vector< Circle*> & c,
                             std::vector< Circle*> & cborder)
    {
        for (int i = 0; i < cborder.size(); ++i)
        {
            for (int j = 0; j < c.size(); ++j)
            {
                if (cborder[i] != c[j])
                {
                    cborder[i]->checkCollision(*c[j]);
                }
            }
        }
    }
    
    void printTree(Node * n)
    {
        if (n == NULL)
        {
            std::cout << "* ";
        }
        else
        {
            printTree(n->getLeft1());
            printTree(n->getLeft2());
            printTree(n->getRight1());
            printTree(n->getRight2());
        }
        if (n != NULL)
        {
        std::cout << "<Node " << n
                  << ", parent:" << n->getParent()
                  << ", left1: " << n->getLeft1()
                  << ", left2: " << n->getLeft2()
                  << ", right1: " << n->getRight1()
                  << ", right2: " << n->getRight2()
                  << ">\n";
        }
    }

private:
    Node * n0;
    int numNodes;
};
