//
// Created by Mohammed Boujemaoui on 16/10/2017.
//

#ifndef EDSP_IFFT_H
#define EDSP_IFFT_H



#include "base/algoritmtemplate.h"
#include "math/container.hpp"
#include <fftw3.h>
#include <assert.h>

EDSP_BEGING_NAMESPACE
    namespace transforms {
        CLASS_TEMPLATE_CONTAINER(Container)
        class IFFT {
        public:
            EDSP_DISABLE_COPY(IFFT)
            explicit IFFT(size_t size) { setSize(size); }
            virtual ~IFFT() { fftw_destroy_plan(fftwPlan); }
            void setSize(size_t size) {
                input.resize(size);
                output.resize(size);
                fftwPlan = fftw_plan_dft_1d(static_cast<int>(size),
                                            reinterpret_cast<fftw_complex *>(&input[0]),
                                            reinterpret_cast<fftw_complex *>(&output[0]),
                                            FFTW_BACKWARD, FFTW_ESTIMATE);

                math::set(input, static_cast<typename Container::value_type>(0));
                math::set(output, static_cast<typename Container::value_type>(0));
            }

            const Container &compute(const Container&data) {
                assert(data.size() <= input.size());
                std::copy(data.begin(), data.end(), input.begin());
                fftw_execute(fftwPlan);
                return output;
            }
        private:
            fftw_plan fftwPlan{nullptr};
            Container  input;
            Container  output;
        };
    }
EDSP_END_NAMESPACE

#endif //EDSP_IFFT_H
