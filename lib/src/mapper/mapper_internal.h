/** Mapper works with quadtree internally */
typedef struct Node {

    struct Node *children[4];
    int z_order_start;
    int z_order_end;
    MaptileValueEnum value;

    } Node;

typedef int (*node_cb_func)(Node **node);

extern int _mapper_add_children(Node * node);
extern int _mapper_remove_children(Node * node);
extern int _mapper_get_node(float x, float y, Node **node, bool add_nodes, Node **parent, int *depth);
extern int _mapper_flatten_node(Node * node);
extern int _mapper_visit_graph(Node *node,
                         node_cb_func callback_func);
extern bool _mapper_node_has_children(Node * node);

extern int _mapper_get_z_order(const int x, const int y, int * z);
extern int _mapper_get_xy_from_z_order(const int z, int * x, int * y);

extern inline unsigned int _mapper_index_from_abs_coords(unsigned int ax,
                                                         unsigned int ay,
                                                         unsigned int max_depth,
                                                         unsigned int current_depth);
