#pragma once






namespace physic {

	// Overloap?
	// these two object should have two member function
	// get_bounds()->fRect   and   is_collid(const fRect&).
	template<typename O1, typename O2>
	bool is_collid(O1& p1, O2& p2) {

		if ((p1.get_bounds().is_collid(p2.get_bounds()))
			|| (p2.get_bounds().is_collid(p1.get_bounds())))
			return true;
		return false;
	}

	// treatment of collision between two object
	template<typename O1, typename O2>
	void collision_process(O1& q1, O2& q2, float aproxi = 2.f) {

		auto p1 = q1.get_position();
		auto p2 = q2.get_position();
		float lx = int(p1.x - p2.x) < 0 ? -1.0 : int(p1.x - p2.x) == 0 ? 0.f : 1.f;
		float ly = int(p1.y - p2.y) < 0 ? -1.0 : int(p1.y - p2.y) == 0 ? 0.f : 1.f;
		q1.set_position(p1.x + lx / aproxi, p1.y + ly / aproxi);
		q2.set_position(p2.x - lx / aproxi, p2.y - ly / aproxi);

	}



}