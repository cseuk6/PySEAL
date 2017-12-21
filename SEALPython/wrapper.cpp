#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "seal/bigpoly.h"
#include "seal/bigpolyarray.h"
#include "seal/biguint.h"
#include "seal/chooser.h"
#include "seal/ciphertext.h"
#include "seal/decryptor.h"
#include "seal/encoder.h"
#include "seal/encryptor.h"
#include "seal/encryptionparams.h"
#include "seal/evaluator.h"
#include "seal/keygenerator.h"
#include "seal/memorypoolhandle.h"
#include "seal/plaintext.h"
#include "seal/defaultparams.h"
#include "seal/publickey.h"
#include "seal/secretkey.h"

namespace py = pybind11;

using namespace pybind11::literals;
using namespace seal;
using namespace std;

// http://pybind11.readthedocs.io/en/stable/classes.html

PYBIND11_MODULE(seal, m) {

  py::class_<BigPoly>(m, "BigPoly")
    .def(py::init<>())
    .def("to_string", &BigPoly::to_string,
         "Returns a human-readable string description of the BigPoly");

  py::class_<BigPolyArray>(m, "BigPolyArray")
    .def(py::init<>());

  py::class_<BigUInt>(m, "BigUInt")
    .def(py::init<>())
    .def("to_double", &BigUInt::to_double,
          "Returns the BigUInt value as a double. Note that precision may be lost during the conversion.")
    .def("significant_bit_count", (int (BigUInt::*)()) &BigUInt::significant_bit_count, "Returns the value of the current SmallModulus");

  py::class_<ChooserEvaluator>(m, "ChooserEvaluator")
    .def(py::init<>());

  py::class_<Ciphertext>(m, "Ciphertext")
    .def(py::init<>())
    .def(py::init<const MemoryPoolHandle &>())
    .def("size", &Ciphertext::size, "Returns the capacity of the allocation");

  py::class_<Decryptor>(m, "Decryptor")
    .def(py::init<const SEALContext &, const SecretKey &>())
    .def(py::init<const SEALContext &, const SecretKey &, const MemoryPoolHandle &>())
    .def("decrypt", (void (Decryptor::*)(const Ciphertext &, Plaintext &, const MemoryPoolHandle &)) &Decryptor::decrypt,
        "Decrypts a ciphertext and writes the result to a given destination.")
    .def("decrypt", (void (Decryptor::*)(const Ciphertext &, Plaintext &)) &Decryptor::decrypt,
        "Decrypts a ciphertext and writes the result to a given destination.")
    .def("invariant_noise_budget", (int (Decryptor::*)(const Ciphertext &))
        &Decryptor::invariant_noise_budget, "Returns noise budget")
    .def("invariant_noise_budget", (int (Decryptor::*)(const Ciphertext &, const MemoryPoolHandle &))
        &Decryptor::invariant_noise_budget, "Returns noise budget");

  py::class_<Encryptor>(m, "Encryptor")
    .def(py::init<const SEALContext &, const PublicKey &>())
    .def(py::init<const SEALContext &, const PublicKey &, const MemoryPoolHandle &>())
    .def(py::init<const Encryptor &>())
    .def(py::init<Encryptor &>())
    .def("encrypt", (void (Encryptor::*)(const Plaintext &, Ciphertext &,
        const MemoryPoolHandle &)) &Encryptor::encrypt,
        "Encrypts a plaintext and writes the result to a given destination")
    .def("encrypt", (void (Encryptor::*)(const Plaintext &, Ciphertext &)) &Encryptor::encrypt,
        "Encrypts a plaintext and writes the result to a given destination");

  py::class_<EncryptionParameters>(m, "EncryptionParameters")
    .def(py::init<>())
    .def(py::init<const EncryptionParameters &>())
    .def("plain_modulus", &EncryptionParameters::plain_modulus, "Returns the plaintext modulus")
    .def("set_coeff_modulus",
        (void (EncryptionParameters::*)(const std::vector<SmallModulus> &)) &EncryptionParameters::set_coeff_modulus,
        "Set coefficient modulus parameter")
    .def("set_plain_modulus",
        (void (EncryptionParameters::*)(const SmallModulus &)) &EncryptionParameters::set_plain_modulus,
        "Set plaintext modulus parameter")
    .def("set_plain_modulus",
        (void (EncryptionParameters::*)(std::uint64_t)) &EncryptionParameters::set_plain_modulus,
        "Set plaintext modulus parameter")
    .def("set_poly_modulus",
        (void (EncryptionParameters::*)(const BigPoly &)) &EncryptionParameters::set_poly_modulus,
        "Set polynomial modulus parameter")
    .def("set_poly_modulus",
        (void (EncryptionParameters::*)(const std::string &)) &EncryptionParameters::set_poly_modulus,
        "Set polynomial modulus parameter");

  py::class_<EncryptionParameterQualifiers>(m, "EncryptionParameterQuailifers");

  py::class_<EvaluationKeys>(m, "EvaluationKeys")
    .def(py::init<>())
    .def("decomposition_bit_count", &EvaluationKeys::decomposition_bit_count, "Returns the decomposition bit count");

  py::class_<Evaluator>(m, "Evaluator")
    .def(py::init<const SEALContext &>())
    .def(py::init<const SEALContext &, const MemoryPoolHandle &>())
    .def("square", (void (Evaluator::*)(Ciphertext &)) &Evaluator::square,
        "Squares a ciphertext")
    .def("square", (void (Evaluator::*)(Ciphertext &, const MemoryPoolHandle &)) &Evaluator::square,
        "Squares a ciphertext")
    .def("negate", (void (Evaluator::*)(Ciphertext &)) &Evaluator::negate,
        "Negates a ciphertext")
    .def("negate", (void (Evaluator::*)(const Ciphertext &, Ciphertext &)) &Evaluator::negate,
        "Negates a ciphertext and writes to a given destination")
    .def("add", (void (Evaluator::*)(const Ciphertext &, const Ciphertext &,
        Ciphertext &)) &Evaluator::add,
        "Adds two ciphertexts and writes to a given destination")
    .def("add", (void (Evaluator::*)(Ciphertext &, const Ciphertext &)) &Evaluator::add,
        "Adds two ciphertexts and writes output over the first ciphertext")
    .def("sub", (void (Evaluator::*)(Ciphertext &, const Ciphertext &)) &Evaluator::sub,
        "Subtracts two ciphertexts and writes output over the first ciphertext")
    .def("sub", (void (Evaluator::*)(const Ciphertext &, const Ciphertext &,
        Ciphertext &)) &Evaluator::sub,
        "Subtracts two ciphertexts and writes to a given destination")
    .def("multiply", (void (Evaluator::*)(Ciphertext &, const Ciphertext &,
        const MemoryPoolHandle &)) &Evaluator::multiply,
        "Multiplies two ciphertexts and writes output over the first ciphertext")
    .def("multiply", (void (Evaluator::*)(Ciphertext &, const Ciphertext &)) &Evaluator::multiply,
        "Multiplies two ciphertexts and writes output over the first ciphertext")
    .def("multiply", (void (Evaluator::*)(const Ciphertext &, const Ciphertext &,
        Ciphertext &, const MemoryPoolHandle &)) &Evaluator::multiply,
        "Multiplies two ciphertexts and writes to a given destination")
    .def("multiply", (void (Evaluator::*)(const Ciphertext &, const Ciphertext &,
        Ciphertext &)) &Evaluator::multiply,
        "Multiplies two ciphertexts and writes to a given destination")
    .def("relinearize", (void (Evaluator::*)(Ciphertext &, const EvaluationKeys &,
        const MemoryPoolHandle &)) &Evaluator::relinearize, "Relinearizes a ciphertext")
    .def("relinearize", (void (Evaluator::*)(Ciphertext &, const EvaluationKeys &))
        &Evaluator::relinearize, "Relinearizes a ciphertext")
    .def("relinearize", (void (Evaluator::*)(const Ciphertext &, const EvaluationKeys &,
        Ciphertext &, const MemoryPoolHandle &)) &Evaluator::relinearize,
        "Relinearizes a ciphertext and writes to a given destination")
    .def("relinearize", (void (Evaluator::*)(const Ciphertext &, const EvaluationKeys &,
        Ciphertext &)) &Evaluator::relinearize,
        "Relinearizes a ciphertext and writes to a given destination");

  py::class_<IntegerEncoder>(m, "IntegerEncoder")
    .def(py::init<const SmallModulus &, std::uint64_t, const MemoryPoolHandle &>())
    .def(py::init<const SmallModulus &, std::uint64_t>())
    .def(py::init<const SmallModulus &, const MemoryPoolHandle &>())
    .def(py::init<const SmallModulus &>())
    .def("encode", (Plaintext (IntegerEncoder::*)(std::uint64_t)) &IntegerEncoder::encode, "Encode integer")
    .def("encode", (void (IntegerEncoder::*)(std::uint64_t, Plaintext &)) &IntegerEncoder::encode, "Encode integer and store in given destination")
    .def("encode", (Plaintext (IntegerEncoder::*)(std::int64_t)) &IntegerEncoder::encode, "Encode integer")
    .def("encode", (void (IntegerEncoder::*)(std::int64_t, Plaintext &)) &IntegerEncoder::encode, "Encode integer and store in given destination")
    .def("encode", (Plaintext (IntegerEncoder::*)(const BigUInt &)) &IntegerEncoder::encode, "Encode integer")
    .def("encode", (void (IntegerEncoder::*)(const BigUInt &, Plaintext &)) &IntegerEncoder::encode, "Encode integer and store in given destination")
    .def("encode", (Plaintext (IntegerEncoder::*)(std::int32_t)) &IntegerEncoder::encode, "Encode integer")
    .def("encode", (Plaintext (IntegerEncoder::*)(std::uint32_t)) &IntegerEncoder::encode, "Encode integer")
    .def("encode", (void (IntegerEncoder::*)(std::int32_t, Plaintext &)) &IntegerEncoder::encode, "Encode integer and store in given destination")
    .def("encode", (void (IntegerEncoder::*)(std::uint32_t, Plaintext &)) &IntegerEncoder::encode, "Encode integer and store in given destination")
    .def("decode_biguint", (void (IntegerEncoder::*)(const Plaintext &, BigUInt &)) &IntegerEncoder::decode_biguint, "Decode a plaintext polynomial and store in a given destination")
    .def("decode_biguint", (BigUInt (IntegerEncoder::*)(const Plaintext &)) &IntegerEncoder::decode_biguint, "Decode a plaintext polynomial")
    .def("decode_int64", (std::int64_t (IntegerEncoder::*)(Plaintext &)) &IntegerEncoder::decode_int64, "Decode a plaintext polynomial")
    .def("decode_int32", (std::int32_t (IntegerEncoder::*)(Plaintext &)) &IntegerEncoder::decode_int32, "Decode a plaintext polynomial")
    .def("decode_uint64", (std::uint64_t (IntegerEncoder::*)(Plaintext &)) &IntegerEncoder::decode_uint64, "Decode a plaintext polynomial")
    .def("decode_uint32", (std::uint32_t (IntegerEncoder::*)(Plaintext &)) &IntegerEncoder::decode_uint32, "Decode a plaintext polynomial");
    //.def("decode_int32", &IntegerEncoder::decode_int32,
    //    "Decodes a plaintext polynomial and returns the result as std::uint32_t");

  py::class_<KeyGenerator>(m, "KeyGenerator")
    .def(py::init<const SEALContext &>())
    .def(py::init<const SEALContext &, const MemoryPoolHandle &>())
    .def(py::init<const SEALContext &, const SecretKey &, const PublicKey &, const MemoryPoolHandle &>())
    .def("generate_evaluation_keys", (void (KeyGenerator::*)(int, int,
        EvaluationKeys &)) &KeyGenerator::generate_evaluation_keys,
        "Generates the specified number of evaluation keys")
    .def("generate_evaluation_keys", (void (KeyGenerator::*)(int,
        EvaluationKeys &)) &KeyGenerator::generate_evaluation_keys,
        "Generates the specified number of evaluation keys")
    .def("public_key", &KeyGenerator::public_key, "Returns public key")
    .def("secret_key", &KeyGenerator::secret_key, "Returns secret key");

  py::class_<MemoryPoolHandle>(m, "MemoryPoolHandle")
    .def(py::init<>())
    .def(py::init<const MemoryPoolHandle &>())
    .def_static("acquire_global", &MemoryPoolHandle::Global,
               "Returns a MemoryPoolHandle pointing to the global memory pool");

  py::class_<Plaintext>(m, "Plaintext")
     .def(py::init<>())
     .def(py::init<const BigPoly &>())
     .def(py::init<const std::string &, const MemoryPoolHandle &>())
     .def(py::init<const std::string &>())
     .def("to_string", &Plaintext::to_string, "Returns the plaintext as a formatted string");

  py::class_<PublicKey>(m, "PublicKey")
     .def(py::init<>());

  py::class_<SecretKey>(m, "SecretKey")
     .def(py::init<>());

  py::class_<SEALContext>(m, "SEALContext")
     .def(py::init<const EncryptionParameters &>())
     .def(py::init<const EncryptionParameters &, const MemoryPoolHandle &>())
     .def("noise_standard_deviation", (double (SEALContext::*)()) &SEALContext::noise_standard_deviation,
        "Returns the maximum deviation of the noise distribution that was given in the encryption parameters")
     .def("total_coeff_modulus", (const BigUInt & (SEALContext::*)()) &SEALContext::total_coeff_modulus,
        "Returns a constant reference to a pre-computed product of all primes in the coefficient modulus.")
     .def("poly_modulus", (const BigPoly & (SEALContext::*)()) &SEALContext::poly_modulus, "Returns a constant reference to the polynomial modulus")
     .def("plain_modulus", (const SmallModulus & (SEALContext::*)()) &SEALContext::plain_modulus, "Returns a constant reference to the plaintext modulus");

  py::class_<SmallModulus>(m, "SmallModulus")
      .def(py::init<>())
      .def(py::init<std::uint64_t>())
      .def("value", (std::uint64_t (SmallModulus::*)()) &SmallModulus::value, "Returns the value of the current SmallModulus");

  m.def("coeff_modulus_128", &coeff_modulus_128, "Returns the default coefficients modulus for a given polynomial modulus degree.");
  m.def("dbc_max", &dbc_max, "Return dbc max value.");
}
