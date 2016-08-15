/*
 * MCBHistoFactory.hh
 *
 *  Created on: Jul 26, 2016
 *      Author: Hovanes Egiyan
 */

// Class that allows us to clone MCBHisto s
#ifndef MCBHISTOFACTORY_HH_
#define MCBHISTOFACTORY_HH_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <regex>
#include <iterator>
#include <functional>

#include <memory>

//using namespace std;
//using namespace std::placeholders;

// partial specialization trick to remove constness of a method
template<typename T>
struct function_remove_const;
template<typename R, typename ... Args>
struct function_remove_const<R( Args... )> {
    using type = R(Args...);
};
template<typename R, typename ... Args>
struct function_remove_const<R( Args... ) const> {
    using type = R(Args...);
};

template<char* const TYPE>
class MCBHistoFactory {
public:
    template<class HISTO>
    MCBHistoFactory( HISTO hist ) :
            self_( std::make_shared<Model<HISTO>>( hist ) ) {
    }

    virtual ~MCBHistoFactory() = default;

    virtual void* Clone( const std::string& name ) const {
        return self_->Clone( name );
    }
    template<char* const NEWTYPE>
    static inline void* makeClone( const MCBHistoFactory<NEWTYPE>& factory, const std::string& name ) {
        return factory.Clone( std::string( name ) );
    }
private:
    struct Concept {
        virtual ~Concept() = default;
        virtual void* Clone( const std::string& name ) const = 0;
    };

    template<class HISTO>
    class Model : public Concept {
    public:
        Model( HISTO data ) :
                data_( data ) {
        }
        virtual void* Clone( const std::string& name ) const {
            HISTO& tmpObj = const_cast<HISTO&>( data_ );
            std::function<function_remove_const<void* ( const std::string& ) const>::type> tmpFunc = std::bind(
                    &HISTO::Clone, &tmpObj, std::placeholders::_1 );
            return tmpFunc( name );
        }
    private:
        HISTO data_;
    };

protected:
    std::shared_ptr<const Concept> self_;
};

#endif /* MCBHISTOFACTORY_HH_ */
