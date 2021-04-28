/*
阅读TensorFlow关于constant_folding（常量折叠）的源码时，观察到了一个lambda表达式的用法。

功能：
有一个函数ReverseDFS，遍历图中的所以节点，找出能够进行常量折叠的节点，把可以折叠的节点放入一个
容器中，再做一些标记。

tensorflow\core\common_runtime\constant_folding.cc
void FindConstantFoldableNodes(
    const Graph* graph, const ConstantFoldingOptions& opts,
    std::vector<Node*>* nodes,
    std::unordered_map<const Node*, gtl::FlatSet<Node*>>* constant_control_deps,
    std::unordered_map<const Node*, std::vector<Tensor>>*
        shape_replacement_map) {
  bool internal_node_inserted = false;
  // Walk the nodes in data flow order.
  ReverseDFS(
      *graph, 
      nullptr,
      [nodes, constant_control_deps, shape_replacement_map,
       &internal_node_inserted, &opts](Node* n) {
        ConsiderConstantFoldableNode(n, opts, nodes, constant_control_deps,
                                     shape_replacement_map,
                                     &internal_node_inserted);
      },
      NodeComparatorName());
  // If we have inserted just leaf level nodes, then there is nothing to fold.
  if (!internal_node_inserted) {
    nodes->clear();
    constant_control_deps->clear();
  }
}

ReverseDFS的函数签名：
void ReverseDFS(const Graph& g, 
                const std::function<void(Node*)>& enter,
                const std::function<void(Node*)>& leave,
                const NodeComparator& stable_comparator)

其中，leave只是一个function，其模板参数是void(Node*)。

但是，使用lambda表达式，就可以在leave中调用ConsiderConstantFoldableNode这个函数，
同时不必把复杂leave的函数签名设置的很复杂。
*/