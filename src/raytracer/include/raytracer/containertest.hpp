#include <functional>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

class container
{
  std::vector<int> ints_;
  std::vector<std::string> strings_;
  std::vector<char> chars_;

public:
  class iterator
  {
    container& cont_;
    std::vector<int>::iterator ints_iterator_;
    std::vector<std::string>::iterator strings_iterator_;
    std::vector<char>::iterator chars_iterator_;

    iterator(container& container, std::vector<int>::iterator ints_iterator)
      : cont_{container}
      , ints_iterator_{ints_iterator}
      , strings_iterator_{container.strings_.end()}
      , chars_iterator_{container.chars_.end()}
    {
    }

    iterator(
          container& container,
          std::vector<std::string>::iterator strings_iterator)
      : cont_{container}
      , ints_iterator_{container.ints_.end()}
      , strings_iterator_{strings_iterator}
      , chars_iterator_{container.chars_.end()}
    {
    }

    iterator(container& container, std::vector<char>::iterator chars_iterator)
      : cont_{container}
      , ints_iterator_{container.ints_.end()}
      , strings_iterator_{container.strings_.end()}
      , chars_iterator_{chars_iterator}
    {
    }

  public:
    ~iterator() = default;

    iterator(const iterator& rhs)
      : cont_{rhs.cont_}
      , ints_iterator_{rhs.ints_iterator_}
      , strings_iterator_{rhs.strings_iterator_}
      , chars_iterator_{rhs.chars_iterator_}
    {
    }

    iterator&
    operator++()
    {
      if (ints_iterator_ != cont_.ints_.end())
      {
        ++ints_iterator_;

        if (ints_iterator_ == cont_.ints_.end())
          strings_iterator_ = cont_.strings_.begin();
      }

      else if (strings_iterator_ != cont_.strings_.end())
      {
        ++strings_iterator_;

        if (strings_iterator_ == cont_.strings_.end())
          chars_iterator_ = cont_.chars_.begin();
      }

      else if (chars_iterator_ != cont_.chars_.end())
        ++chars_iterator_;

      return *this;
    }

    bool
    operator!=(iterator rhs) const
    {
      if (ints_iterator_ != rhs.ints_iterator_)
        return true;

      if (strings_iterator_ != rhs.strings_iterator_)
        return true;

      if (chars_iterator_ != rhs.chars_iterator_)
        return true;

      return false;
    }

    std::variant<
          std::reference_wrapper<int>, std::reference_wrapper<std::string>,
          std::reference_wrapper<char>>
    operator*()
    {
      if (ints_iterator_ != cont_.ints_.end())
        return std::ref(*ints_iterator_);

      if (strings_iterator_ != cont_.strings_.end())
        return std::ref(*strings_iterator_);

      return std::ref(*chars_iterator_);
    }

    friend class container;
  };

  iterator
  begin() noexcept
  {
    return iterator(*this, ints_.begin());
  }

  iterator
  end() noexcept
  {
    return iterator(*this, chars_.end());
  }

  template<typename... Args>
  void
  add(int first, Args... args)
  {
    ints_.push_back(first);
    (ints_.push_back(args), ...);
  }

  template<typename... Args>
  void
  add(std::string first, Args... args)
  {
    strings_.push_back(first);
    (strings_.push_back(args), ...);
  }

  template<typename... Args>
  void
  add(char first, Args... args)
  {
    chars_.push_back(first);
    (chars_.push_back(args), ...);
  }

  void
  print(std::ostream& out)
  {
    out << "ints:|";

    for (const auto& val: ints_)
      out << val << '|';

    out << "\nstrings: |";

    for (const auto& val: strings_)
      out << val << '|';

    out << "\nchars: |";

    for (const auto& val: chars_)
      out << val << '|';

    out << "\n";
  }
};
