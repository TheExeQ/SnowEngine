#include "Math.h"

namespace Snow
{
	namespace Math
	{
		bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale)
		{
			{
				// From glm::decompose in matrix_decompose.inl

				using namespace glm;
				using T = float;

				mat4 LocalMatrix(transform);

				// Normalize the matrix.
				if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
					return false;

				// First, isolate perspective.  This is the messiest.
				if (
					epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
					epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
					epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
				{
					// Clear the perspective partition
					LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
					LocalMatrix[3][3] = static_cast<T>(1);
				}

				// Next take care of translation (easy).
				translation = vec3(LocalMatrix[3]);
				LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

				vec3 Row[3], Pdum3;

				// Now get scale and shear.
				for (length_t i = 0; i < 3; ++i)
					for (length_t j = 0; j < 3; ++j)
						Row[i][j] = LocalMatrix[i][j];

				// Compute X scale factor and normalize first row.
				scale.x = length(Row[0]);
				Row[0] = detail::scale(Row[0], static_cast<T>(1));
				scale.y = length(Row[1]);
				Row[1] = detail::scale(Row[1], static_cast<T>(1));
				scale.z = length(Row[2]);
				Row[2] = detail::scale(Row[2], static_cast<T>(1));

				rotation.y = asin(-Row[0][2]);
				if (cos(rotation.y) != 0)
				{
					rotation.x = atan2(Row[1][2], Row[2][2]);
					rotation.z = atan2(Row[0][1], Row[0][0]);
				}
				else
				{
					rotation.x = atan2(-Row[2][0], Row[1][1]);
					rotation.z = 0;
				}

				return true;
			}
		}

		glm::mat4 ConvertAssimpMat4ToGlmMat4(const aiMatrix4x4& aMatrix)
		{
			glm::mat4 result;
			//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
			result[0][0] = aMatrix.a1; result[1][0] = aMatrix.a2; result[2][0] = aMatrix.a3; result[3][0] = aMatrix.a4;
			result[0][1] = aMatrix.b1; result[1][1] = aMatrix.b2; result[2][1] = aMatrix.b3; result[3][1] = aMatrix.b4;
			result[0][2] = aMatrix.c1; result[1][2] = aMatrix.c2; result[2][2] = aMatrix.c3; result[3][2] = aMatrix.c4;
			result[0][3] = aMatrix.d1; result[1][3] = aMatrix.d2; result[2][3] = aMatrix.d3; result[3][3] = aMatrix.d4;
			return result;
		}

		glm::mat4 ConvertOzzMat4ToGlmMat4(const ozz::math::Float4x4& aMatrix)
		{
			glm::mat4 result;

			result[0][0] = aMatrix.cols[0].m128_f32[0]; result[1][0] = aMatrix.cols[1].m128_f32[0]; result[2][0] = aMatrix.cols[2].m128_f32[0]; result[3][0] = aMatrix.cols[3].m128_f32[0];
			result[0][1] = aMatrix.cols[1].m128_f32[1]; result[1][1] = aMatrix.cols[1].m128_f32[1]; result[2][1] = aMatrix.cols[2].m128_f32[1]; result[3][1] = aMatrix.cols[3].m128_f32[1];
			result[0][2] = aMatrix.cols[2].m128_f32[2]; result[1][2] = aMatrix.cols[1].m128_f32[2]; result[2][2] = aMatrix.cols[2].m128_f32[2]; result[3][2] = aMatrix.cols[3].m128_f32[2];
			result[0][3] = aMatrix.cols[3].m128_f32[3]; result[1][3] = aMatrix.cols[1].m128_f32[3]; result[2][3] = aMatrix.cols[2].m128_f32[3]; result[3][3] = aMatrix.cols[3].m128_f32[3];
			
			return result;
		}
	}
}