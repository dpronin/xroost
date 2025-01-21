#pragma once

#include <cstddef>

#include <sys/types.h>

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <stack>
#include <utility>

#include <xroost/memory/unique_ptr.hpp>

namespace xroost {

template <typename T, typename Allocator = std::allocator<T>> class avl_tree {
private:
  using value_type = T;
  using allocator_type = Allocator;

  class node final {
    friend class avl_tree<value_type, allocator_type>;

  public:
    explicit node(value_type &&value) : value_(std::move_if_noexcept(value)) {}
    explicit node(value_type const &value) : value_(value) {}
    ~node() = default;

    node(node const &) = delete;
    node &operator=(node const &) = delete;

    node(node &&) = delete;
    node &operator=(node &&) = delete;

    size_t fix_height() {
      auto const l_height{p_left_ ? p_left_->fix_height() : 0};
      auto const r_height{p_right_ ? p_right_->fix_height() : 0};
      return height_ = std::max(l_height, r_height) + 1;
    }

    ssize_t bfactor() const {
      return (p_left_ ? p_left_->height_ : 0) -
             (p_right_ ? p_right_->height_ : 0);
    }

    value_type const &value() const { return value_; }
    size_t height() const { return height_; }

  private:
    value_type value_;
    size_t height_{1};
    memory::unique_ptr<node, std::function<void(struct node *)>> p_left_,
        p_right_;
  };

  class IPrinterStrategy {
  public:
    IPrinterStrategy() = default;
    virtual ~IPrinterStrategy() = default;

    IPrinterStrategy(IPrinterStrategy const &) = delete;
    IPrinterStrategy &operator=(IPrinterStrategy const &) = delete;

    IPrinterStrategy(IPrinterStrategy &&) = delete;
    IPrinterStrategy &operator=(IPrinterStrategy &&) = delete;

    virtual void print(node const &node) = 0;
  };

  class PreorderPrinterStrategy : public IPrinterStrategy {
  public:
    PreorderPrinterStrategy() = default;
    ~PreorderPrinterStrategy() override = default;

    PreorderPrinterStrategy(PreorderPrinterStrategy const &) = delete;
    PreorderPrinterStrategy &
    operator=(PreorderPrinterStrategy const &) = delete;

    PreorderPrinterStrategy(PreorderPrinterStrategy &&) = delete;
    PreorderPrinterStrategy &operator=(PreorderPrinterStrategy &&) = delete;

  private:
    void print(node const &node) override {
      std::stack<struct node const *> nodes;
      nodes.push(&node);
      while (!nodes.empty()) {
        auto p_node{nodes.top()};
        nodes.pop();
        std::cout << p_node->value() << " ";
        if (p_node->p_right_)
          nodes.push(p_node->p_right_.get());
        if (p_node->p_left_)
          nodes.push(p_node->p_left_.get());
      }
      std::cout << std::endl;
    }
  };

  class InorderPrinterStrategy : public IPrinterStrategy {
  public:
    InorderPrinterStrategy() = default;
    ~InorderPrinterStrategy() override = default;

    InorderPrinterStrategy(InorderPrinterStrategy const &) = delete;
    InorderPrinterStrategy &operator=(InorderPrinterStrategy const &) = delete;

    InorderPrinterStrategy(InorderPrinterStrategy &&) = delete;
    InorderPrinterStrategy &operator=(InorderPrinterStrategy &&) = delete;

  private:
    void print(node const &node) override {
      std::stack<struct node const *> nodes;
      auto *p_node{&node};
      while (p_node || !nodes.empty()) {
        while (p_node) {
          nodes.push(p_node);
          p_node = p_node->p_left_.get();
        }

        p_node = nodes.top();
        nodes.pop();
        std::cout << p_node->value() << " ";

        p_node = p_node->p_right_.get();
      }
      std::cout << std::endl;
    }
  };

  class PostorderPrinterStrategy : public IPrinterStrategy {
  public:
    PostorderPrinterStrategy() = default;
    ~PostorderPrinterStrategy() override = default;

    PostorderPrinterStrategy(PostorderPrinterStrategy const &) = delete;
    PostorderPrinterStrategy &
    operator=(PostorderPrinterStrategy const &) = delete;

    PostorderPrinterStrategy(PostorderPrinterStrategy &&) = delete;
    PostorderPrinterStrategy &operator=(PostorderPrinterStrategy &&) = delete;

  private:
    void print(node const &node) override {
      std::stack<struct node const *> first, second;
      first.push(&node);
      while (!first.empty()) {
        auto p_node{first.top()};
        first.pop();
        second.push(p_node);
        if (p_node->p_left_)
          first.push(p_node->p_left_.get());
        if (p_node->p_right_)
          first.push(p_node->p_right_.get());
      }

      while (!second.empty()) {
        auto p_node{second.top()};
        second.pop();
        std::cout << p_node->value() << " ";
      }
    }
  };

public:
  avl_tree() = default;
  ~avl_tree() = default;

  enum class printer { prefix, infix, postfix };

  bool insert(value_type const &value) noexcept {
    return insert_(p_root_, value);
  }

  void print_height() const noexcept {
    auto p_left{p_root_ ? p_root_->p_left_ : 0};
    auto p_right{p_root_ ? p_root_->p_right_ : 0};
    std::cout << "leftHeight == " << (p_left ? p_left->height_ : 0)
              << "; rightHeight == " << (p_right ? p_right->height_ : 0)
              << std::endl;
  }

  void print() {
    if (p_printer_ && p_root_)
      p_printer_->print(*p_root_);
    std::cout << std::endl;
  }

  void set_printer(printer id) {
    switch (id) {
    case printer::prefix:
      p_printer_.reset(new PreorderPrinterStrategy{});
      break;
    case printer::infix:
      p_printer_.reset(new InorderPrinterStrategy{});
      break;
    case printer::postfix:
      p_printer_.reset(new PostorderPrinterStrategy{});
      break;
    default:
      break;
    }
  }

private:
  void right_rotate_(
      memory::unique_ptr<node, std::function<void(struct node *)>> &p_node) {
    auto q_node{std::move(p_node->p_left_)};
    p_node->p_left_ = std::move(q_node->p_right_);
    q_node->p_right_ = std::move(p_node);
    p_node = std::move(q_node);
  }

  void left_rotate_(
      memory::unique_ptr<node, std::function<void(struct node *)>> &p_node) {
    auto q_node{std::move(p_node->p_right_)};
    p_node->p_right_ = std::move(q_node->p_left_);
    q_node->p_left_ = std::move(p_node);
    p_node = std::move(q_node);
  }

  void balance_(
      memory::unique_ptr<node, std::function<void(struct node *)>> &p_node) {
    p_node->fix_height();
    auto const bfactor{p_node->bfactor()};
    // right subtree is heavier than the left by 2, required to rotate big to
    // the right
    if (-2 == bfactor) {
      // left subtree of the subtree is heavier than the right, required to
      // rotate small to the right
      if (p_node->p_right_->bfactor() > 0)
        right_rotate_(p_node->p_right_);
      left_rotate_(p_node);
    }
    // left subtree is heavier than the right by 2, required to rotate big to
    // the left
    else if (2 == bfactor) {
      // right subtree of the subtree is heavier than the left, required to
      // rotate small to the left
      if (p_node->p_left_->bfactor() < 0)
        left_rotate_(p_node->p_left_);
      right_rotate_(p_node);
    }
  }

  bool
  insert_(memory::unique_ptr<node, std::function<void(struct node *)>> &p_node,
          value_type const &value) {
    bool result{false};

    if (!p_node) {
      p_node = {
          new (allocator_.allocate(1)) node{value},
          [this](node *p_node) {
            std::destroy_at(p_node);
            allocator_.deallocate(p_node, 1);
          },
      };
      result = true;
    } else if (value == p_node->value()) {
      result = false;
    } else if (value < p_node->value()) {
      result = insert_(p_node->p_left_, value);
      if (result)
        balance_(p_node->p_left_);
      balance_(p_node);
    } else {
      result = insert_(p_node->p_right_, value);
      if (result)
        balance_(p_node->p_right_);
      balance_(p_node);
    }

    return result;
  }

private:
  memory::unique_ptr<node, std::function<void(struct node *)>> p_root_;
  memory::unique_ptr<IPrinterStrategy> p_printer_;
  std::allocator_traits<allocator_type>::template rebind_alloc<node> allocator_;
};

} // namespace xroost
