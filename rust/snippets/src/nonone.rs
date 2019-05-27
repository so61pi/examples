// We shouldn't use this anyway. It's quite compilcated than normal. Instead we
// can use bitflags to mark which field has value (or not). The only benefit of
// presented code is we cannot in anyway set all the fields to None and get away
// with that, even when we add new field and forget to add new check for it in
// new function.

#[derive(Debug)]
pub enum NoNone<U, V> {
	Some { val: U, next: Option<V> },
	None { next: V },
}

impl<U, V> NoNone<U, V> {
	pub fn value(&self) -> Option<&U> {
		if let NoNone::Some { val, .. } = &self {
			Some(val)
		} else {
			None
		}
	}

	pub fn next(&self) -> Option<&V> {
		match &self {
			NoNone::Some { next, .. } => next.as_ref(),
			NoNone::None { next } => Some(next),
		}
	}
}

fn newtop<U, V>(ou: Option<U>, ov: Option<V>) -> NoNone<U, V> {
	if let Some(u) = ou {
		NoNone::Some { val: u, next: ov }
	} else {
		NoNone::None {
			next: ov.expect("at least u or v must be set"),
		}
	}
}

fn newsub<U, V>(ou: Option<U>, ov: Option<V>) -> Option<NoNone<U, V>> {
	if let Some(u) = ou {
		Some(NoNone::Some { val: u, next: ov })
	} else if let Some(v) = ov {
		Some(NoNone::None { next: v })
	} else {
		None
	}
}

#[derive(Debug, PartialEq, Clone)]
struct Email(String);

#[derive(Debug, PartialEq, Clone)]
struct Phone(String);

#[derive(Debug, PartialEq, Clone)]
struct Address(String);

#[derive(Debug)]
struct ContactInfo(NoNone<Email, NoNone<Phone, Address>>);

impl ContactInfo {
	fn new(email: Option<Email>, phone: Option<Phone>, address: Option<Address>) -> ContactInfo {
		if email.is_none() && phone.is_none() && address.is_none() {
			panic!("at least one contact option must be set");
		}

		ContactInfo(newtop(email, newsub(phone, address)))
	}

	fn email(&self) -> Option<&Email> {
		self.0.value()
	}

	fn phone(&self) -> Option<&Phone> {
		self.0.next().and_then(NoNone::value)
	}

	fn address(&self) -> Option<&Address> {
		self.0.next().and_then(NoNone::next)
	}
}

#[cfg(test)]
mod tests {
	use super::*;

	#[test]
	fn test_contact_1() {
		let email = Email("email".to_string());
		let phone = Phone("phone".to_string());
		let address = Address("address".to_string());
		let contact = ContactInfo::new(
			Some(email.clone()),
			Some(phone.clone()),
			Some(address.clone()),
		);

		assert_eq!(contact.email().unwrap(), &email);
		assert_eq!(contact.phone().unwrap(), &phone);
		assert_eq!(contact.address().unwrap(), &address);
	}

	#[test]
	fn test_contact_2() {
		let phone = Phone("phone".to_string());
		let address = Address("address".to_string());
		let contact = ContactInfo::new(None, Some(phone.clone()), Some(address.clone()));

		assert!(contact.email().is_none());
		assert_eq!(contact.phone().unwrap(), &phone);
		assert_eq!(contact.address().unwrap(), &address);
	}

	#[test]
	#[should_panic(expected = "at least one contact option must be set")]
	fn test_contact_3() {
		let _contact = ContactInfo::new(None, None, None);
	}
}