//
// Created by federico on 20/06/2020.
//

#ifndef CONSTRAINTS_SOLVER_VARIABLE_H
#define CONSTRAINTS_SOLVER_VARIABLE_H

#include <cassert>
#include <set>
#include <map>
#include <utility>

namespace csl {

    template<class type>
    class Variable{

    public:

        template<typename Iter>
        Variable(Iter begin, Iter end, std::string id="") : id_string(std::move(id)) {
            domain = std::set<type>(begin, end);
            for (auto value : domain)
                removal_count[value] = 0;
        }

        Variable(std::initializer_list<type> list, const std::string& id="") : Variable(list.begin(), list.end(), id) {}

        ///var must have a unique value in its domain,
        ///the value is removed from this domain
        bool remove(const Variable* var) {

            auto type_var = static_cast<const Variable<type>*>(var);

            assert(var->has_unique_value());

            if (removal_count.count(type_var->unique_value().value())) {
                removal_count[type_var->unique_value().value()]++;
                return domain.erase(type_var->unique_value().value());
            } else
                return false;

        }

        ///var must have a unique value in its domain,
        ///the value is added back in this domain if the removal being reverted actually removed the value
        void revert(const Variable<type>* var) {

            assert(var->has_unique_value());

            if (auto type_var = dynamic_cast<const Variable<type>*>(var); type_var)
                if (removal_count.count(type_var->unique_value().value())) {
                    if (--removal_count[type_var->unique_value().value()] == 0)
                        domain.insert(type_var->unique_value().value());
                }
        }

        ///This method is used to iterate over possible choices of a single value from the domain (i.e. to enter choice mode).
        ///The first time it returns true and chooses a value (the domain temporarily only contains that value).
        ///Subsequent calls return true, and change the value in the domain.
        ///The final call returns false, and sets the variable back to its original state.
        bool next_choice() {

            if(!choice_mode) { //enter choice mode
                choice_mode = true;
                original_domain = std::set<type>(domain.begin(), domain.end());
                iterator = original_domain.begin();
                for (type v:original_domain)
                    removal_count[v] = 1;
            } else { //increase iterator
                removal_count[*iterator] = 1;
                iterator++;
            }

            if (iterator == original_domain.end()) { //exit choice mode
                domain = std::set<type>(original_domain.begin(), original_domain.end());
                original_domain.clear();
                choice_mode = false;
                for (type v:domain)
                    removal_count[v] = 0;
                return false;
            } else { //update domain
                domain.clear();
                domain.insert(*iterator);
                removal_count[*iterator] = 0;
                return true;
            }

        }

        [[nodiscard]] size_t inline domain_size() const {
            return domain.size();
        }

        [[nodiscard]] bool inline has_unique_value() const {
            return domain_size() == 1;
        }

        std::optional<type> inline unique_value() const{
            return has_unique_value() ? std::optional<type>(*domain.begin()) : std::optional<type>();
        }

        std::string to_string() {
            return id_string;
        }

    private:

        ///The domain is the set of currently acceptable values
        std::set<type> domain;

        ///This map counts the number of removal requests for each variable, even those that came when
        std::map<type, int> removal_count;


        bool choice_mode = false;

        std::set<type> original_domain;

        ///Iterator over the original domain when in choice mode
        typename std::set<type>::iterator iterator;


        std::string id_string;

    };
}



#endif //CONSTRAINTS_SOLVER_VARIABLE_H
